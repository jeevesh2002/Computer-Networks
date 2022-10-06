#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "DNS.h"

int main(int argc, char **argv)
{

    Entry table[MAX_DOMAIN], result;
    bzero(table, MAX_DOMAIN * sizeof(Entry));

    if (argc < 2)
    {
        fprintf(stderr, "Error: Enter port number for server as second argument!\n");
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);
    int sockfd, len;
    struct sockaddr_in servaddr, cliadrr;
    char buff[30];
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        fprintf(stderr, "Error: Socket creation failed!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket creation successfull!\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        fprintf(stderr, "Error: Socket bind failed!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket bind successfull\n");

    len = sizeof(cliadrr);

    createEntry(table, "google.com", "192.168.1.1");
    createEntry(table, "yahoo.com", "194.12.34.12");
    createEntry(table, "google.com", "17.10.23.123");

    printTable(table);

    string domain, address, opt;

    while (1)
    {
        recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr *)&cliadrr, &len);
        result = getAddress(table, buff);
        sendto(sockfd, &result, sizeof(Entry), MSG_CONFIRM, (struct sockaddr *)&cliadrr, len);

        int flag = 0;

        printf("Do you want to modify (yes/no): ");
        scanf("%s", opt);
        if (strcmp(opt, "yes") == 0)
        {
            printf("Enter domain: ");
            scanf("%s", domain);
            do
            {
                printf("Enter IP address: ");
                scanf("%s", address);
                flag = createEntry(table, domain, address);
                switch (flag)
                {
                case 1:
                    break; // Correct IP
                case -1:
                    printf("Invalid IP address!\n");
                    break;
                case -2:
                    printf("Duplicate IP address!\n");
                    break;
                default:
                    printf("Error!\n");
                }
            } while (flag != 1);

            printf("Updated table\n");
            printTable(table);
        }
    }

    close(sockfd);
}
