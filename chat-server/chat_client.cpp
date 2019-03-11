#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
#include <iostream>


int main() {

    int sock;
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo("127.0.0.1", "8080", &hints, &res);
    
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);


    bool isLoggedIn = false;

    while (!isLoggedIn) {
        char buffer1[1024] = {0};
        std::string name;
        std::cout << "Name: ";
        std::cin >> name;    

        std::string message = "HELLO-FROM " + name + "\n";
        send(sock, message.c_str(), message.length(),0);

        recv(sock, buffer1, 1024, 0);
        std::cout << buffer1;
        std::string resp(buffer1);

        if (resp.substr(0,5) == "HELLO") {
            isLoggedIn = true;
        }
    }

    std::cout << "I am logged in!\n";
    while(1) {
        char buffer2[1024] = {0};
        std::string command;
        std::cout << "Enter command: ";
        std::cin >> command;
        if (command == "!who") {
            send(sock, "WHO\n",5, 0);    
        } else {
            send(sock, command.c_str(), command.length(), 0);
        }
        recv(sock, buffer2, 1024, 0);
        std::cout << buffer2 << std::endl;
        
    }
    return 0'
}
