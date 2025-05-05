#include "person_in_room.h"

PersonInRoom::PersonInRoom(
    boost::asio::io_context& io_context,
    boost::asio::strand<boost::asio::io_context::executor_type>& strand,
    ChatRoom& room)
    : socket_(io_context), strand_(strand), room_(room) {}

tcp::socket& PersonInRoom::socket() { return socket_; }

void PersonInRoom::start() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(nickname_),
      boost::asio::bind_executor(
          strand_,
          [self = shared_from_this()](const boost::system::error_code& ec,
                                      size_t) { self->nicknameHandler(ec); }));
}

void PersonInRoom::onMessage(std::array<char, MAX_MSG_SIZE>& msg) {
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress) {
    boost::asio::async_write(
        socket_, boost::asio::buffer(write_msgs_.front()),
        boost::asio::bind_executor(
            strand_,
            [self = shared_from_this()](const boost::system::error_code& ec,
                                        size_t) { self->writeHandler(ec); }));
  }
}

void PersonInRoom::nicknameHandler(const boost::system::error_code& error) {
  if (!error) {
    if (strlen(nickname_.data()) <= MAX_NICKNAME - 2) {
      strcat(nickname_.data(), ": ");
    } else {
      nickname_[MAX_NICKNAME - 2] = ':';
      nickname_[MAX_NICKNAME - 1] = ' ';
    }

    room_.enter(shared_from_this(), std::string(nickname_.data()));

    boost::asio::async_read(
        socket_, boost::asio::buffer(read_msg_),
        boost::asio::bind_executor(
            strand_,
            [self = shared_from_this()](const boost::system::error_code& ec,
                                        size_t) { self->readHandler(ec); }));
  } else {
    room_.leave(shared_from_this());
  }
}

void PersonInRoom::readHandler(const boost::system::error_code& error) {
  if (!error) {
    room_.broadcast(read_msg_, shared_from_this());

    boost::asio::async_read(
        socket_, boost::asio::buffer(read_msg_),
        boost::asio::bind_executor(
            strand_,
            [self = shared_from_this()](const boost::system::error_code& ec,
                                        size_t) { self->readHandler(ec); }));
  } else {
    room_.leave(shared_from_this());
  }
}

void PersonInRoom::writeHandler(const boost::system::error_code& error) {
  if (!error) {
    write_msgs_.pop_front();

    if (!write_msgs_.empty()) {
      boost::asio::async_write(
          socket_, boost::asio::buffer(write_msgs_.front()),
          boost::asio::bind_executor(
              strand_,
              [self = shared_from_this()](const boost::system::error_code& ec,
                                          size_t) { self->writeHandler(ec); }));
    }
  } else {
    room_.leave(shared_from_this());
  }
}
