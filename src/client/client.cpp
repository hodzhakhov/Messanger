#include "client.h"

Client::Client(const std::array<char, MAX_NICKNAME>& nickname,
               asio::io_context& io_context,
               const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context),
      work_guard_(asio::make_work_guard(io_context)) {
  std::copy_n(nickname.data(), nickname.size(), nickname_.data());
  read_msg_.fill('\0');
  asio::async_connect(socket_, endpoints,
                      [this](const boost::system::error_code& ec, auto) {
                        this->onConnect(ec);
                      });
}

void Client::write(const std::array<char, MAX_MSG_SIZE>& msg) {
  asio::post(io_context_, [this, msg]() { this->innerWrite(msg); });
}

void Client::close() {
  asio::post(io_context_, [this]() { this->innerClose(); });
}

void Client::onConnect(const boost::system::error_code& error) {
  if (!error) {
    asio::async_write(socket_, asio::buffer(nickname_),
                      [this](const boost::system::error_code& ec, auto) {
                        this->readHandler(ec);
                      });
  }
}

void Client::readHandler(const boost::system::error_code& error) {
  if (!error) {
    std::cout << read_msg_.data() << std::endl;
    asio::async_read(socket_, asio::buffer(read_msg_),
                     [this](const boost::system::error_code& ec, auto) {
                       this->readHandler(ec);
                     });
  } else {
    innerClose();
  }
}

void Client::innerWrite(std::array<char, MAX_MSG_SIZE> msg) {
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress) {
    asio::async_write(socket_, asio::buffer(write_msgs_.front()),
                      [this](const boost::system::error_code& ec, auto) {
                        this->writeHandler(ec);
                      });
  }
}

void Client::writeHandler(const boost::system::error_code& error) {
  if (!error) {
    write_msgs_.pop_front();
    if (!write_msgs_.empty()) {
      asio::async_write(socket_, asio::buffer(write_msgs_.front()),
                        [this](const boost::system::error_code& ec, auto) {
                          this->writeHandler(ec);
                        });
    }
  } else {
    innerClose();
  }
}

void Client::innerClose() {
  if (socket_.is_open()) {
    socket_.close();
  }
  work_guard_.reset();
}
