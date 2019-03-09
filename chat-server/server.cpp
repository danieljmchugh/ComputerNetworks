/* https://www.bogotobogo.com/cplusplus/sockets_server_client.php */

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
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // socket(int domain, int type, int protocol)
    sockFd =  socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol(???)

    portNum = 8080;

    /* setup the host_addr structure for use in bind call */
    serv_addr.sin_family = AF_INET;  // server byte order

    // automatically be filled with current host's IP address
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    
    // convert short integer value for port must be converted into network byte order
    serv_addr.sin_port = htons(portNum);
    
    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length) 
    bind(sockFd,(struct sockaddr *) &serv_addr, sizeof(serv_addr));  // (struct sockaddr *) &serv_addr : read more on the data structs
    
    // This listen() call tells the socket to listen to the incoming connections.
    // The listen() function places all incoming connection into a backlog queue
    // until accept() call accepts the connection.
    // Here, we set the maximum size for the backlog queue to 5.
    listen(sockFd,5);


    // The accept() call actually accepts an incoming connection
    clilen = sizeof(cli_addr);

    // This accept() function will write the connecting client's address info 
    // into the the address structure and the size of that structure is clilen.
    // The accept() returns a new socket file descriptor for the accepted connection.
    // So, the original socket file descriptor can continue to be used 
    // for accepting new connections while the new socker file descriptor is used for
    // communicating with the connected client.
    newSockFd = accept(sockFd, (struct sockaddr *) &cli_addr, &clilen);
    if (newSockFd < 0) 
        cout << "ERROR on accept\n";

    printf("server: got connection from %s port %d\n",
        inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));


    // This send() function sends the 13 bytes of the string to the new socket
    send(newSockFd, "Hello, world!\n", 13, 0);

    return 0;
}