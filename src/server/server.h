#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <list>

#include "chat_room/chat_room.h"
#include "person_in_room/person_in_room.h"

class server {
 public:
  server(boost::asio::io_context& io_context,
         boost::asio::strand<boost::asio::io_context::executor_type>& strand,
         const tcp::endpoint& endpoint);
  ~server();

 private:
  void run();
  void onAccept(std::shared_ptr<personInRoom> new_participant,
                const boost::system::error_code& error);

  boost::asio::io_context& io_context_;
  boost::asio::strand<boost::asio::io_context::executor_type>& strand_;
  tcp::acceptor acceptor_;
  chatRoom room_;
};

#endif
