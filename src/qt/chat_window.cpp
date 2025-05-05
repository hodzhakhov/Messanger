#include "chat_window.h"

ChatWindow::ChatWindow(const std::string& nickname, const std::string& host,
                       const std::string& port, QWidget* parent)
    : QWidget(parent), socket_(io_context_), nickname_(nickname) {
  setupUI();
  connectToServer(host, port);
  startReaderThread();
}

ChatWindow::~ChatWindow() {
  if (socket_.is_open()) {
    boost::system::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close(ec);
  }
  if (reader_thread_.joinable()) {
    reader_thread_.join();
  }
}

void ChatWindow::setupUI() {
  chatView_ = new QTextEdit(this);
  chatView_->setReadOnly(true);

  input_ = new QLineEdit(this);
  sendButton_ = new QPushButton("Send", this);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(chatView_);
  layout->addWidget(input_);
  layout->addWidget(sendButton_);

  connect(sendButton_, &QPushButton::clicked, this, &ChatWindow::sendMessage);
  connect(input_, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
}

void ChatWindow::connectToServer(const std::string& host,
                                 const std::string& port) {
  tcp::resolver resolver(io_context_);
  auto endpoints = resolver.resolve(host, port);
  boost::asio::connect(socket_, endpoints);

  std::array<char, MAX_NICKNAME> nick{};
  std::strncpy(nick.data(), nickname_.c_str(), MAX_NICKNAME - 1);
  boost::asio::write(socket_, boost::asio::buffer(nick));
}

void ChatWindow::startReaderThread() {
  reader_thread_ = std::thread([this]() {
    std::array<char, MAX_MSG_SIZE> msg{};
    while (socket_.is_open()) {
      try {
        boost::system::error_code ec;
        size_t length =
            boost::asio::read(socket_, boost::asio::buffer(msg), ec);

        if (ec) {
          if (ec == boost::asio::error::eof) {
            break;
          } else {
            throw boost::system::system_error(ec);
          }
        }

        std::string clean_msg(msg.data(), length);
        clean_msg.erase(std::find(clean_msg.begin(), clean_msg.end(), '\0'),
                        clean_msg.end());

        QMetaObject::invokeMethod(
            this, "appendMessage", Qt::QueuedConnection,
            Q_ARG(QString, QString::fromStdString(clean_msg)));
      } catch (...) {
        break;
      }
    }
  });
}

void ChatWindow::sendMessage() {
  std::string text = input_->text().toStdString();
  if (!text.empty()) {
    std::array<char, MAX_MSG_SIZE> msg{};
    std::strncpy(msg.data(), text.c_str(), MAX_MSG_SIZE - 1);
    write(msg);
    input_->clear();
  }
}

void ChatWindow::appendMessage(const QString& msg) { chatView_->append(msg); }

void ChatWindow::write(const std::array<char, MAX_MSG_SIZE>& msg) {
  boost::asio::write(socket_, boost::asio::buffer(msg));
}
