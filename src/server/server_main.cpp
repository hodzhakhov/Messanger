#include "server.h"

int main(int argc, char* argv[]) {
  try {
    if (argc < 2) {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    auto io_context = std::make_shared<boost::asio::io_context>();
    auto strand = std::make_shared<
        boost::asio::strand<boost::asio::io_context::executor_type>>(
        boost::asio::make_strand(*io_context));

    std::cout << "server starts\n";

    std::list<std::shared_ptr<Server>> servers;
    for (int i = 1; i < argc; ++i) {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      auto a_server = std::make_shared<Server>(*io_context, *strand, endpoint);
      servers.push_back(a_server);
    }

    boost::thread_group workers;
    for (int i = 0; i < 1; ++i) {
      workers.create_thread([io_context]() { io_context->run(); });
    }

    workers.join_all();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
