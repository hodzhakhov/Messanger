#ifndef CLIENT_H
#define CLIENT_H

#include <array>
#include <boost/asio.hpp>
#include <deque>
#include <iostream>

#include "../common.h"

using boost::asio::ip::tcp;
namespace asio = boost::asio;

class Client {
 public:
  Client(const std::array<char, MAX_NICKNAME>& nickname,
         asio::io_context& io_context,
         const tcp::resolver::results_type& endpoints);

  void write(const std::array<char, MAX_MSG_SIZE>& msg);
  void close();

 private:
  void onConnect(const boost::system::error_code& error);
  void readHandler(const boost::system::error_code& error);
  void innerWrite(std::array<char, MAX_MSG_SIZE> msg);
  void writeHandler(const boost::system::error_code& error);
  void innerClose();

  asio::io_context& io_context_;
  tcp::socket socket_;
  asio::executor_work_guard<asio::io_context::executor_type> work_guard_;
  std::array<char, MAX_MSG_SIZE> read_msg_{};
  std::deque<std::array<char, MAX_MSG_SIZE>> write_msgs_;
  std::array<char, MAX_NICKNAME> nickname_{};
};

#endif
