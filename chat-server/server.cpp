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



void * socketThread(void *arg) {
    int currSocket = *((int *)arg);
    char buffer[4096] = {0};

    int bytes_recv = recv(currSocket, buffer, 4096, 0);
    std::string buf(buffer);
    if (bytes_recv > 0) {
        // chech if received name is taken
        std::string client_name = buf.substr(11,(buf.length() - 12));
        if (!clients.checkIfTaken(client_name)) {
            // add client to list
            Client currClient(client_name, currSocket, true);
            clients.add(currClient);

            std::cout << "New client:\n";
            //currClient.getAllInfo();

            clients.print();

            std::string line = "HELLO " + currClient.getName() + "\n";
            send(currSocket, line.c_str(), line.length(), 0);

            // while client is loggin in.
            while(1) {
                char buffer2[4096] = {0};
                int bytes_recv = recv(currSocket, buffer2, 4096, 0);
                std::string buf2(buffer2);
                
                // if message received
                if (bytes_recv > 0) {
                    
                    if (buf2 == "QUIT\n") {
                        break;
                    } 
                    else if (buf2 == "WHO\n") {
                        send(currSocket, clients.who().c_str(), clients.who().length(), 0);
                        std::cout << currClient.getName() << "\t> !who\n";
                    }
                    else if (buf2.substr(0,4) == "SEND") {
                        int handle = clients.find(buf2);

                        if (handle > 0) {
                            std::cout << currClient.getName() << " sent message" << std::endl;

                            std::string line = "DELIVERY " + currClient.getName() + " " + clients.extractMessage(buf2) + '\n';

                            send(handle, line.c_str(), line.length(), 0);

                            send(currClient.getHandle(), "SEND-OK\n", 9, 0);
                        } else {
                            send(currClient.getHandle(), "UNKNOWN\n", 9, 0);
                        }
                    } 
                    else {
                        send(currSocket, "BAD-RQST-BODY\n", 15, 0);
                    }
                } 
                // if nothing received 
                else {
                  break;  
                }
            }
            
            // When client quits
            std::cout << currClient.getName() << " left.\n";
            clients.remove(currClient.getName());

        } else {
            // close socket if taken
            send(currSocket, "IN-USE\n", 8, 0);
            std::cout << "New client's name was taken.\n";
        }
    }

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