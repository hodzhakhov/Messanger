#include "messanger.h"

User::User(std::string name) : name(std::move(name)) {}

std::string User::getName() const { return name; }

void MessageQueue::sendMessage(const Message &message) {
  messages.push(message);
}

std::vector<Message> MessageQueue::receiveMessages(const std::string &user) {
  std::vector<Message> userMessages;
  std::queue<Message> tempQueue;

  while (!messages.empty()) {
    Message msg = messages.front();
    messages.pop();
    if (msg.receiver == user) {
      userMessages.push_back(msg);
    } else {
      tempQueue.push(msg);
    }
  }
  messages = std::move(tempQueue);
  return userMessages;
}

void Server::addUser(const std::string &name) {
  users[name] = std::make_shared<User>(name);
}

void Server::sendMessage(const std::string &sender, const std::string &receiver,
                         const std::string &text) {
  if (users.find(sender) != users.end() &&
      users.find(receiver) != users.end()) {
    messageQueue.sendMessage({sender, receiver, text});
  } else {
    std::cerr << "Ошибка: один из пользователей не найден!" << std::endl;
  }
}

void Server::receiveMessages(const std::string &user) {
  if (users.find(user) != users.end()) {
    std::vector<Message> msgs = messageQueue.receiveMessages(user);
    for (const auto &msg : msgs) {
      std::cout << "От: " << msg.sender << " -> " << msg.receiver << " : "
                << msg.text << std::endl;
    }
  } else {
    std::cerr << "Ошибка: пользователь не найден!" << std::endl;
  }
}
