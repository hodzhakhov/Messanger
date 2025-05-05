#include "client.h"

int main(int argc, char* argv[]) {
  try {
    if (argc != 4) {
      std::cerr << "Usage: chat_client <nickname> <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[2], argv[3]);

    std::array<char, MAX_NICKNAME> nickname{};
    std::strncpy(nickname.data(), argv[1], MAX_NICKNAME - 1);
    nickname[MAX_NICKNAME - 1] = '\0';

    Client cli(nickname, io_context, endpoints);

    std::thread t([&io_context]() { io_context.run(); });

    std::array<char, MAX_MSG_SIZE> msg{};

    while (true) {
      msg.fill('\0');
      if (!std::cin.getline(msg.data(),
                            MAX_MSG_SIZE - PADDING - MAX_NICKNAME)) {
        std::cin.clear();
      }
      std::string message(msg.data());
      if (message == "/quit") {
        break;
      }

      cli.write(msg);
    }

    cli.close();
    t.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
