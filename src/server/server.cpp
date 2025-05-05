#include "server.h"

server::server(
    boost::asio::io_context& io_context,
    boost::asio::strand<boost::asio::io_context::executor_type>& strand,
    const tcp::endpoint& endpoint)
    : io_context_(io_context),
      strand_(strand),
      acceptor_(io_context, endpoint) {
  run();
}

server::~server() { acceptor_.close(); }

void server::run() {
  auto new_participant =
      std::make_shared<personInRoom>(io_context_, strand_, room_);

  acceptor_.async_accept(new_participant->socket(),
                         boost::asio::bind_executor(
                             strand_, [this, new_participant](
                                          const boost::system::error_code& ec) {
                               this->onAccept(new_participant, ec);
                             }));
}

void server::onAccept(std::shared_ptr<personInRoom> new_participant,
                      const boost::system::error_code& error) {
  if (!error) {
    new_participant->start();
  }

  run();
}
