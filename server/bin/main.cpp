#include "lib/messanger.h"

int main() {
    Server server;
    server.addUser("Alice");
    server.addUser("Bob");
    
    server.sendMessage("Alice", "Bob", "Привет, Боб!");
    server.sendMessage("Alice", "Bob", "Как дела, Боб?");
    server.sendMessage("Bob", "Alice", "Привет, Алиса!");
    
    std::cout << "\nСообщения для Bob:" << std::endl;
    server.receiveMessages("Bob");
    
    std::cout << "\nСообщения для Alice:" << std::endl;
    server.receiveMessages("Alice");
    
    return 0;
}
