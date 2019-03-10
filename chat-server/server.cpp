// https://www.bogotobogo.com/cplusplus/sockets_server_client.php 
// https://github.com/RedAndBlueEraser/c-multithreaded-client-server/blob/master/server.c
// https://dzone.com/articles/parallel-tcpip-socket-server-with-multi-threading
#include <cstdio>
#include <vector>
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

std::vector<std::string> clients; 



void * socketThread(void *arg) {
    int currSocket = *((int *)arg);
    char buffer[1024] = {0};
    
    std::string client_name = "Dan";
    clients.push_back(client_name);
    
    //char buffer[1024] = {0};
    recv(currSocket, buffer, 1024, 0);
    // std::string messageS(buffer);    
    
    // std::cout << "String: " << messageS;
    // std::cout << "------" << std::endl; // test for \n in messageS
    
    if (strcmp("WHO\n",buffer)) {std::cout << "1) WHO\n"; } 
    
    else if (strcmp("WHO\\n",buffer)) {
        std::cout << "Currently online: ";
        //std::cout << "2) WHO\\n\n";
        for (int i = 0; i < clients.size(); i++) {
             std::cout << clients.at(i) << ", ";
        } 
    }
    
    
    else {std::cout << "Neither...\n";}

    // if (messageS.substr(0,10) == "HELLO-FROM") {
    //         client_name = messageS.substr(11,(messageS.length()-11));
    //         for (int i = 0; i < clients.size(); i++) {
    //             if (clients.at(i) == client_name) {
    //                 std::cout << client_name << " is taken!" << std::endl;
    //             }
    //         }
    //         // std::cout << "HELLO " << client_name << std::endl;
    //         // clients.push_back(client_name);
    // }

    // while (recv(currSocket, buffer, 1024, 0) > 0) {
    //      if (messageS == "WHO\n") { //.substr(0,4)
            // std::cout << "Currently online: ";
            // for (int i = 0; i < clients.size(); i++) {
            //     std::cout << clients.at(i) << ", ";
            // } 
    //     } else {
    //         std::cout << messageS;
    //     }
    // }
      

    // Need to close properly
    close(currSocket);
    pthread_exit(NULL);
}


int main() {
    
    int sockFd, newSockFd, portNum;

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr_storage serverStorage;

    //std::vector<int> clients;

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