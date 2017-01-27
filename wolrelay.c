#include "wolrelay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define DEFAULT_PORT "1337"
#define WOL_PACKET_LENGTH 102

//Number of unused bytes in the beginning of WOL packet
#define WOL_PACKET_OFFSET 6

//Length of a MAC address
#define MAC_ADDR_LENGTH 6

//char *inet_ntoa(struct in_addr in);
int main(int argc, char* argv[]){

    const char* hostname = NULL;
    const char* portname = DEFAULT_PORT;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //Allow only ipv4 for our our purpose
    hints.ai_protocol = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

    struct addrinfo* res = NULL;
    int err = getaddrinfo(hostname, portname, &hints, &res);
    if (err != 0) {
        printf("Failed to find local socket addresses. Error code: %d", err);
    }

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1) {
        perror("Failed to create a socket");
    }

    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1 ) {
        perror("Failed to bind a local socket");
    }

    freeaddrinfo(res);

    char buffer[549];
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);
    int s;

    ssize_t count = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&src_addr, &src_addr_len);
    if (count == -1) {
        perror("Error reading from socket");
    } else if (count != WOL_PACKET_LENGTH){
        printf("The packet received is not 102 byte long and is %ld bytes long instead", (long) count);
    } else {
        char host[NI_MAXHOST], service[NI_MAXSERV];
        s = getnameinfo((struct sockaddr *) &src_addr, src_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        printf("Received %ld bytes from %s:%s\n", (long) count, host, service);
        for (int i = 0; i < count; i ++){
            printf("%X", (int)(*(unsigned char*)(&buffer[i])) );
        }
        printf("\n");
    }

    if (isValidMagicPacket(buffer)){
        printf("Magic packet verified\n");
        printf("The mac address is following:\n");

    }

}

int isValidMagicPacket(char* payload){
    
    //Check if first four bytes are 255
    for (int i = 0; i < WOL_PACKET_OFFSET; i++){
        if (payload[i] != (char) 0xFF) return 0;
    }

    //Check if we have the mac address is 16 times
    for (int i = 0; i < 16; i++){
        for (int k = 0; k < MAC_ADDR_LENGTH; k++){
            if (payload[WOL_PACKET_OFFSET + k] != payload[WOL_PACKET_OFFSET + (MAC_ADDR_LENGTH * i) + k])
                return 0;
        }
    }
    return 1;
}