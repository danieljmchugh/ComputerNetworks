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

using namespace std;

int main() {

    int sock;
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char buffer[4096] = {0};
    char buffer2[4096] = {0};
    char buffer3[4096] = {0};
    char buffer4[4096] = {0};
    string msg = "REPORT botid=2437018b280e2f36 os=windows <END>";

    if (getaddrinfo("127.0.0.1", "8080", &hints, &res) == 0) {
        
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        connect(sock, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);

        //send(sock, msg.c_str(), msg.length(), 0);

        recv(sock, buffer, 4096, 0);

        printf("%s", buffer);

    }
    else 
        cout << "Could not find ip\n";
}
