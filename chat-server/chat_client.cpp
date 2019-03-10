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

    // std::string name;
    // std::cout << "Name: ";
    // std::cin >> name;    

    // std::string message = "HELLO-FROM " + name + "\n";
    // send(sock, message.c_str(), message.length(),0);

    // std::string command;
    // std::cin >> command;
    // send(sock, command.c_str(), command.length(),0);
    send(sock, "WHO\n", 5,0);
    //send(sock, "WHO\\n", 6,0);

    // while(1) {
    //     int bytes_recv = 0;
    //     char buffer[1024] = {0};
        
    //     std::cout << "\t>";
    //     std::cin >> command;
        
    //     if (command == "!who") {
    //         int bytes_sent = send(sock, "WHO\n", 5, 0);

    //     } else if (command.at(0) == '@') {
    //         std::string input;
    //         getline(std::cin, input);

    //         std::string message = "SEND " + command.substr(1, command.length()) + " " + input + "\n";
    //         std::cout << message;
    //         int bytes_sent = send(sock, message.c_str(), message.length(), 0);

            
    //         std::cout << "CLIENT: " << message.c_str();
        
    //         bytes_recv = recv(sock, buffer, 1024, 0);
            
    //         std::cout << "SERVER: " << buffer;
    //     }
    // }



}
