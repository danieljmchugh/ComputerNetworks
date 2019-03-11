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

/*
    TODO:
        -implement rmClient func.

*/

bool checkNameTaken(std::string name) {
    for (int i = 0; i < clients.size(); i++) {
        if (clients.at(i) == name) {
            return true;
        }
    }
    return false;
}

// void rmClient(std::string name) {
    
// }

std::string currOnline(){
    std::string list = "WHO-OK ";
    for (int i = 0; i < clients.size(); i++) {
        if (i == (clients.size() -1)) {
            list += "";
        }
        list += (clients.at(i) + ", ");
    }
    return list;
}


void * socketThread(void *arg) {
    int currSocket = *((int *)arg);
    std::string client_name;
    bool isLoggedIn = false;
    

    while (!isLoggedIn) {
        char buffer[1024] = {0};
        recv(currSocket, buffer, 1024, 0);
        std::string messageS(buffer);    
        bool isNameTaken;
        std::cout << "String: " << messageS;
        std::cout << "------" << std::endl; // test for \n in messageS
        
        client_name = messageS.substr(11,(messageS.length()-12));
        std::cout << "\t Name: ---" << client_name << "---" << std::endl;
        isNameTaken = checkNameTaken(client_name);
        if (isNameTaken == false) { 
            clients.push_back(client_name);
            isLoggedIn = true;
            std::string loginMsg = "HELLO " + client_name + '\n';
            send(currSocket,loginMsg.c_str(),loginMsg.length(),0);
        } else if (isNameTaken == true) {
            isLoggedIn = false;
            send(currSocket,"IN-USE\n",7,0);
        }
    }

    if (isLoggedIn) {
        char buffer1[1024] = {0};
        std::cout << "Client " << currSocket << " is logged in as " << client_name << std::endl;
        
        std::string command;
        while(command != "!quit") {
            std::cout << "Waiting for command...\n";
            recv(currSocket, buffer1, 1024, 0);
            command = buffer1;
            if (command == "WHO\n") {
                std::string line = currOnline();
                send(currSocket, line.c_str(), line.length(), 0);
            } else {
                std::cout << command << std::endl;
                send(currSocket, command.c_str(), command.length(), 0);
            }
        }    
    }

    // Need to close properly and remove client
    //rmClient(client_name);
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