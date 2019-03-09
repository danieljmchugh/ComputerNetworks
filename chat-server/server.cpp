#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#include <errno.h>

using namespace std;


/*
    TODO:
        -add error handling
*/


int main() {
    
    int sockFd, newSockFd, portNum;

    struct sockaddr_in serv_addr, cli_addr;

    // socket(int domain, int type, int protocol)
    sockFd =  socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol(???)

    portNum = 2000;

    /* setup the host_addr structure for use in bind call */
    serv_addr.sin_family = AF_INET;  // server byte order

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    
    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(portNum);
    
    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length) 
    bind(sockFd,(struct sockaddr *) &serv_addr, sizeof(serv_addr));  // (struct sockaddr *) &serv_addr : read more on the data structs
    
    return 0;
}