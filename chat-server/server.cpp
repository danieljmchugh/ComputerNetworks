// https://www.bogotobogo.com/cplusplus/sockets_server_client.php 
// https://github.com/RedAndBlueEraser/c-multithreaded-client-server/blob/master/server.c
// https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading
#include <cstdio>
#include <string>
#include <thread>   /* used for threading */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#include <errno.h>

#include <iostream>

#include "client.h"

Clients clients;

/*
    BUG:
        user IN-USE for second client
*/

void * socketThread(void *arg) {
    int currSocket = *((int *)arg);
    std::string client_name;
    bool isLoggedIn = false;
    
    
    Client currClient("-MISSING-", currSocket, isLoggedIn);  // current client object
    std::cout << "Client connected...\n";

    while (!currClient.isLoggedIn()) {
        char buffer[1024] = {0};
        recv(currSocket, buffer, 1024, 0);
        std::cout << "RECEIVED: " << buffer;
        std::string messageS(buffer);    
        bool isNameTaken;
        
        std::cout << strlen(buffer) << std::endl;
        if ((strlen(buffer) > 0)) {
            client_name = messageS.substr(11,(messageS.length()-12));

            
            isNameTaken = clients.checkIfTaken(client_name);
            
            if (isNameTaken == false) { 
                
                currClient.setName(client_name);
                std::cout << "Client set name: " << currClient.getName() << std::endl;
                isLoggedIn = true;
                currClient.isLoggedIn(true);
                
                clients.add(currClient);

                std::string loginMsg = "HELLO " + currClient.getName() + '\n';

                send(currSocket,loginMsg.c_str(),loginMsg.length(),0);

            } else if (isNameTaken == true) {
                //isLoggedIn = false;
                std::cout << "Client set name was taken." << std::endl;
                send(currSocket,"IN-USE\n",7,0);
                //currClient.isLoggedIn(false);
            }
        }
    }

    if (currClient.isLoggedIn()) {
        char buffer1[1024] = {0};
        std::cout << "Client " << currClient.getHandle() << " is logged in as " << currClient.getName() << std::endl;
        
        std::string command = "";
        while(currClient.isLoggedIn()) {
            
            std::cout << "Waiting for command...\n";
            recv(currSocket, buffer1, 1024, 0);
            command = buffer1;

            if (command == "WHO\n") {
                
                std::cout << currClient.getName() << " sent !who" << std::endl;
                std::string line = clients.who();
                send(currSocket, line.c_str(), line.length(), 0);
                
            } 
            else if (command == "QUIT\n") {
                

                currClient.isLoggedIn(false);
                std::cout << currClient.getName() << " quit." << std::endl;
                clients.remove(currClient.getHandle());

                break;

            } 
            else if (command == "ME\n") {

                currClient.getAllInfo();
                send(currSocket, "OK", 3, 0);
                std::cout << currClient.getName() << " sent !me" << std::endl;

            }
            else if (command.substr(0,4) == "SEND") {
                int handle = clients.find(command);

                if (handle > 0) {
                    std::cout << currClient.getName() << " sent message" << std::endl;

                    std::string line = "DELIVERY " + currClient.getName() + " " + clients.extractMessage(command) + '\n';

                    send(handle, line.c_str(), line.length(), 0);

                    send(currClient.getHandle(), "SEND-OK\n", 9, 0);
                } else {
                    send(currClient.getHandle(), "UNKNOWN\n", 9, 0);
                }
            }
            else {
                std::cout << command << std::endl;
                //send(currSocket, command.c_str(), command.length(), 0);
            }
        }    
    }
    std::cout << "Client has left the chat\n";
    // Need to close properly and remove client
    
    close(currSocket);
    pthread_exit(NULL);
}


int main() {
    
    int sockFd, newSockFd, portNum;

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr_storage serverStorage;


    sockFd =  socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol(???)

    portNum = 8080;

    serv_addr.sin_family = AF_INET;  // server byte order
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(portNum);

    bind(sockFd,(struct sockaddr *) &serv_addr, sizeof(serv_addr));  // (struct sockaddr *) &serv_addr : read more on the data structs
    
    // Backlog = 10, dont think this will be any problem
    listen(sockFd, 10);
    
    // The accept() call actually accepts an incoming connection
    clilen = sizeof(cli_addr);

    // Threading: create detached(w/ pthread_detach())
    pthread_t client_thread;

    while(1) {
        //Accept call and create a new socket for the incoming connection
        clilen = sizeof serverStorage;

        newSockFd = accept(sockFd, (struct sockaddr *) &serverStorage, &clilen);
        printf("Got one!\n");

        pthread_create(&client_thread, NULL, socketThread, &newSockFd);        
    }

    return 0;
}