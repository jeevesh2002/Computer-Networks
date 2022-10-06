#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<unistd.h>
#include "ARP.h"
#include<sys/types.h>
#include<arpa/inet.h>
int main(int argc, char **argv){

    if (argc < 2){
        fprintf(stderr, "Enter port number as second argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);

    struct sockaddr_in server, client;
    char buffer[1024];
    int client_sockets[10] = {0}, max, fd, sockfd, newfd, activity;
    int k, i, len, count;
    fd_set newfds;
    
    arp packet, recv_packet;

    packet = createARPPacket(REQ);
    printf("\nDeveloping ARP Request packet\n");
    printPacket(packet);
    printf("\tThe ARP Request packet is broacasted.\n");
    printf("Waiting for ARP Reply...\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        perror("Unable to open socket.\n");
        exit(EXIT_FAILURE);
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("Bind error occurred.\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 10);

    len = sizeof(client);

    while(1){
        FD_ZERO(&newfds);            //Clears socket set.
        FD_SET(sockfd, &newfds);    //Add sockfd to socket set.
        
        max = sockfd;

        for(i = 0; i < 10; i++){
            fd = client_sockets[i];
            
            if(fd > 0){
                FD_SET(fd, &newfds);
            }

            if(fd > max){        //Store the max valued FD.
                max = fd;
            }
        }

        
        //Wait indefinitely till any client pings.
        activity = select(max+1, &newfds, NULL, NULL, NULL);

        if(activity < 0){
            perror("Select error occurred.\n");
            exit(EXIT_FAILURE);
        }

        //if sockfd change => new connection request.
        if(FD_ISSET(sockfd, &newfds)){
            newfd = accept(sockfd, (struct sockaddr*)&client, &len);
            
            if(newfd < 0){
                perror("Unable to accept the new connection.\n");
                exit(EXIT_FAILURE);
            }

            send(newfd,(void*)&packet, sizeof(packet), 0);

            //Add the new client on an empty slot.
            for(i = 0; i < 10; i++){
                if(client_sockets[i] == 0){
                    client_sockets[i] = newfd;
                    break;
                }
            }
        }

        //Broadcast on all established connections
        for(i = 0; i < 10; i++){
            fd = client_sockets[i];
            bzero((void*)&recv_packet, sizeof(recv_packet));

            //Check for change in FD
            if(FD_ISSET(fd, &newfds)){
                recv(fd, (void*)&recv_packet, sizeof(recv_packet), 0);

                //Check ARP response
                if(recv_packet.mode == ACK){
                    printf("\nARP Reply received: \n");
                    printPacket(recv_packet);
                    
                    strcpy(packet.dest_mac, recv_packet.src_mac);
                    packet.mode = DATA;

                    printf("\nSending the packet to: %s\n", packet.dest_mac);

                    send(newfd, (void*)&packet, sizeof(packet), 0);
                    printf("Packet sent: \n");
                    printPacket(packet);
                    exit(EXIT_SUCCESS);
                }
            }
        }

    }
    close(sockfd);
    return 0;
}
