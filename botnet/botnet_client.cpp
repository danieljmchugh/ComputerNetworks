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

    if (getaddrinfo("52.58.97.202", "2357", &hints, &res) == 0) {
        
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        connect(sock, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);

        send(sock, msg.c_str(), msg.length(), 0);

        recv(sock, buffer, 4096, 0);

        printf("%s", buffer);



        send(sock, "UPDATE version=1.33.7 <END>", 28, 0);

        recv(sock, buffer2, 4096, 0);

        printf("%s", buffer2);
    

        
        send(sock, "COMMAND <END>", 14, 0);

        recv(sock, buffer3, 4069, 0); //65536

        printf("%s", buffer3);



        send(sock, "DONE <END>", 11, 0);

        recv(sock, buffer4, 4096, 0);

        printf("%s", buffer4);
    }
    else 
        cout << "Could not find ip\n";
}
