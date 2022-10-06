#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#define SERVER_UDP_PORT 5035
int main(void)
{  int sockfd,csize,arr[30],pre=0,flag,p,i;
   char buffer[1029];
   struct sockaddr_in serveraddr,clientaddr;
   fd_set readfds;
   for(int i=0;i<30;i++)
     arr[i]=0;
   sockfd=socket(AF_INET,SOCK_DGRAM,0);
   printf("%d\n",sockfd);
   serveraddr.sin_family=AF_INET;
   serveraddr.sin_addr.s_addr=INADDR_ANY;
   serveraddr.sin_port=htons(SERVER_UDP_PORT);
   bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
   bzero(&serveraddr, sizeof(struct sockaddr_in));
   listen(sockfd,3);
   FD_ZERO(&readfds);
   while(1)
   {  FD_SET(sockfd,&readfds);
      csize=sizeof(clientaddr);
      select(sockfd+1,&readfds,NULL,NULL,NULL);
      recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&clientaddr,&csize);
      
      p=ntohs(clientaddr.sin_port);
      flag=0;
      printf("%d",p);
      for(i=0;i<30;i++)
        if(arr[i]==p)
        {  p=i+1;
           flag=1;
           break;
        }
      if(flag==0)
      {  arr[pre]=p;
         pre++;
         p=pre;
      }

      printf("\nMessage from Client %d: %s\n",p,buffer);
      csize=sizeof(serveraddr);
      sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,csize);
      printf("\nMessage sent: %s\n\n",buffer);

   }
   close(sockfd);
   return 0;
}
