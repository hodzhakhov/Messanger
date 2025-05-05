#define BOOST_TEST_MODULE QtClientTests
#include <boost/test/included/unit_test.hpp>

#include "../src/qt/chat_window.h"

BOOST_AUTO_TEST_CASE(test_ui_components) {
  int argc = 0;
  QApplication app(argc, nullptr);

  ChatWindow window("test", "localhost", "8888");

  BOOST_CHECK(window.findChild<QTextEdit*>() != nullptr);
  BOOST_CHECK(window.findChild<QLineEdit*>() != nullptr);
  BOOST_CHECK(window.findChild<QPushButton*>() != nullptr);
}

BOOST_AUTO_TEST_CASE(test_message_display) {
  int argc = 0;
  QApplication app(argc, nullptr);

  ChatWindow window("test", "localhost", "8888");
  QTextEdit* chatView = window.findChild<QTextEdit*>();
  BOOST_REQUIRE(chatView != nullptr);

  window.appendMessage("Test message");

  BOOST_CHECK(chatView->toPlainText().contains("Test message"));
}
