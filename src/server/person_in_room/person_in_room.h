#ifndef PERSON_IN_ROOM_H
#define PERSON_IN_ROOM_H

#include <array>
#include <boost/asio.hpp>
#include <deque>
#include <memory>

#include "../chat_room/chat_room.h"
#include "../participant/participant.h"

using boost::asio::ip::tcp;

class PersonInRoom : public Participant,
                     public std::enable_shared_from_this<PersonInRoom> {
 public:
  PersonInRoom(
      boost::asio::io_context& io_context,
      boost::asio::strand<boost::asio::io_context::executor_type>& strand,
      ChatRoom& room);

  tcp::socket& socket();
  void start();
  void onMessage(std::array<char, MAX_MSG_SIZE>& msg) override;

 private:
  void nicknameHandler(const boost::system::error_code& error);
  void readHandler(const boost::system::error_code& error);
  void writeHandler(const boost::system::error_code& error);

  tcp::socket socket_;
  boost::asio::strand<boost::asio::io_context::executor_type>& strand_;
  ChatRoom& room_;
  std::array<char, MAX_NICKNAME> nickname_{};
  std::array<char, MAX_MSG_SIZE> read_msg_{};
  std::deque<std::array<char, MAX_MSG_SIZE>> write_msgs_;
};

#endif
