#include "chat_room.h"

void chatRoom::enter(std::shared_ptr<Participant> participant,
                     const std::string& nickname) {
  participants_.insert(participant);
  name_table_[participant] = nickname;
  for (const auto& msg : recent_msgs_) {
    participant->onMessage(const_cast<std::array<char, MAX_MSG_SIZE>&>(msg));
  }
}

void chatRoom::leave(std::shared_ptr<Participant> participant) {
  participants_.erase(participant);
  name_table_.erase(participant);
}

void chatRoom::broadcast(std::array<char, MAX_MSG_SIZE>& msg,
                         std::shared_ptr<Participant> participant) {
  std::string nickname = name_table_[participant];
  std::array<char, MAX_MSG_SIZE> formatted_msg;

  strcpy(formatted_msg.data(), nickname.c_str());
  strcat(formatted_msg.data(), msg.data());

  recent_msgs_.push_back(formatted_msg);
  while (recent_msgs_.size() > max_recent_msgs) {
    recent_msgs_.pop_front();
  }

  for (const auto& p : participants_) {
    p->onMessage(formatted_msg);
  }
}
