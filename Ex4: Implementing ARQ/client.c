#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include "ARP.h"
#include<arpa/inet.h>
int main(int argc, char **argv){
     if (argc < 2){
        fprintf(stderr, "Enter port number as second argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    struct sockaddr_in server, client;
    char buffer[1024];
    int sockfd, newfd;
    int len, i, count, k;
    arp packet, recv_packet;

    printf("\nEnter the IP Address\t: ");
    scanf("%s", packet.src_ip);
    printf("\nEnter the MAC Address\t: ");
    scanf("%s", packet.src_mac);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        perror("Unable to open socket.\n");
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    len = sizeof(client);

    bzero(&recv_packet, sizeof(recv_packet));
    recv(sockfd, (void*)&recv_packet, sizeof(recv_packet), 0);
    printf("\nARP Request Received: \n");
    printPacket(recv_packet);

    if(strcmp(packet.src_ip, recv_packet.dest_ip) == 0){
        printf("\nIP Address matches.\n");
        packet.mode = ACK;
        strcpy(packet.dest_ip, recv_packet.src_ip);
        strcpy(packet.dest_mac, recv_packet.src_mac);

        send(sockfd, (void*)&packet, sizeof(packet), 0);
        printf("\nARP Reply Sent: \n");
        printPacket(packet);

        bzero(&recv_packet, sizeof(recv_packet));
        recv(sockfd, (void*)&recv_packet, sizeof(recv_packet), 0);
        printf("\nReceived Packet is: \n");
        printPacket(recv_packet);
    }

    else{
        printf("\nIP Address does not match.\n");
    }

    close(sockfd);

    return 0;
}
