#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#define SERVER_UDP_PORT 5035
int main(void)
{  int sockfd,ssize;
   char buffer[1024];
   struct sockaddr_in serveraddr,clientaddr;
   sockfd=socket(AF_INET,SOCK_DGRAM,0);
   serveraddr.sin_family=AF_INET;
   serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   serveraddr.sin_port=htons(SERVER_UDP_PORT);
   while(strcmp(buffer,"Exit\n")!=0)
   {  ssize=sizeof(serveraddr);
      printf("\nClient: ");
      fgets(buffer,1024,stdin);
      sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serveraddr,ssize);
      recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&serveraddr,&ssize);
      printf("\nMessage from server: %s\n",buffer);
   }
   close(sockfd);
}
