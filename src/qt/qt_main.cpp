#include "chat_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  if (argc != 4) {
    qDebug() << "Usage: qt_client <nickname> <host> <port>";
    return 1;
  }

  ChatWindow window(argv[1], argv[2], argv[3]);
  window.setWindowTitle("Chat Client");
  window.resize(400, 300);
  window.show();

  return app.exec();
}
