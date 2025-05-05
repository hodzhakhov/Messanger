#define BOOST_TEST_MODULE ServerTests
#include "../src/server/server.h"

#include <boost/asio.hpp>
#include <boost/test/included/unit_test.hpp>

using boost::asio::ip::tcp;

BOOST_AUTO_TEST_CASE(test_message_processing) {
  boost::asio::io_context io_context;
  auto strand = boost::asio::make_strand(io_context);
  ChatRoom room;

  class mock_participant : public Participant {
   public:
    void onMessage(std::array<char, MAX_MSG_SIZE>& msg) override {
      last_message = msg.data();
    }
    std::string last_message;
  };

  auto participant = std::make_shared<mock_participant>();
  room.enter(participant, "test_user: ");

  std::array<char, MAX_MSG_SIZE> test_msg{};
  strcpy(test_msg.data(), "Hello");

  room.broadcast(test_msg, participant);
  BOOST_CHECK_EQUAL(participant->last_message, "test_user: Hello");
}
