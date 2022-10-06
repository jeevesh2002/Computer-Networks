#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX 100
#define PORT 8000
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX],file[MAX];
    int n;
    bzero(buff, sizeof(buff));
    printf("Enter the name of the file : ");
    scanf("%s",buff);
    
    bzero(file,sizeof(file));
    strcpy(file,"copyof");
    strcat(file,buff);
    //strcat(file,"1");
    
    
    send(sockfd, buff, sizeof(buff),0);
    
    
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, sizeof(buff),0);
    
        
    if (strcmp(buff, "Unable to open file") == 0) {
        printf("Unable to open file\n");
        return;
    }
    else
        printf("The contents of the file are : \n%s", buff);
        
    int fd = open(file, O_WRONLY | O_CREAT);
    if(fd==-1){
        strcpy(buff,"Unable to open file");
    }
    
    int size;
    for(int i=0 ; buff[i]!='\0' ; i++)
    size++;
    write(fd,buff,size);
    close(fd);        
        
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
