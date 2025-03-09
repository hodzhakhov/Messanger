#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

struct Message {
  std::string sender;
  std::string receiver;
  std::string text;
};

class User {
 public:
  explicit User(std::string name);
  std::string getName() const;

 private:
  std::string name;
};

class MessageQueue {
 public:
  void sendMessage(const Message &message);

  std::vector<Message> receiveMessages(const std::string &user);

 private:
  std::queue<Message> messages;
};

class Server {
 public:
  void addUser(const std::string &name);

  void sendMessage(const std::string &sender, const std::string &receiver,
                   const std::string &text);

  void receiveMessages(const std::string &user);

 private:
  std::unordered_map<std::string, std::shared_ptr<User>> users;
  MessageQueue messageQueue;
};
