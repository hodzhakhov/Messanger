#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <boost/asio.hpp>

#include "../common.h"

using boost::asio::ip::tcp;

class ChatWindow : public QWidget {
  Q_OBJECT
 public:
  ChatWindow(const std::string& nickname, const std::string& host,
             const std::string& port, QWidget* parent = nullptr);
  ~ChatWindow();

 public slots:
  void sendMessage();
  void appendMessage(const QString& msg);

 private:
  void setupUI();
  void connectToServer(const std::string& host, const std::string& port);
  void startReaderThread();
  void write(const std::array<char, MAX_MSG_SIZE>& msg);

  boost::asio::io_context io_context_;
  tcp::socket socket_;
  std::string nickname_;
  std::thread reader_thread_;

  QTextEdit* chatView_;
  QLineEdit* input_;
  QPushButton* sendButton_;
};

#endif
