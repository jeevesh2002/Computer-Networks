#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <math.h>
#include<time.h>

#define MAX 50
#define PORT 8080

int find_r(int m){
    int r=1;
    while(pow(2, r) < r + m + 1)
        r += 1;
    return r;
}
int r_position(int n){
    return (ceil(log2(n))==floor(log2(n)));
}
int to_binary(int n){
    int num=0,rem,i=0;
    while(n>0){
        rem=n%2;
        num+= rem*pow(10,i);
        n=n/2;
        i++;
    }
    return num;
}
int ispresent(int num,int pos)
{
    int rem;
    for(int i = 0; i < pos; i++)
    {
        rem = num % 10;
        num = num / 10;
    }
    if(rem == 1)
        return 1;
    else
        return 0;
}
int main(int argc, char **argv)
{
    int sockfd,newfd, len;
    struct sockaddr_in server_address, client_address;
    char buff[30];
    //socket establishing
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("SOCKET NOT ESTABLISHED.\n");
        exit(EXIT_FAILURE);
    }
    //clear address pointer
    bzero(&server_address, sizeof(server_address));
    // assign IP, PORT
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    //bind the socket with the IP
    if ((bind(sockfd, (const struct sockaddr *)&server_address, sizeof(server_address))) != 0)
    {
        perror("Bind error occurred.\n");
        exit(EXIT_FAILURE);
    }
    listen(sockfd, 2);
    //start
    long data;
    char data_s[MAX];
    int code[MAX];
    printf("Enter input data: ");
    scanf("%lu",&data);
    sprintf(data_s,"%lu",data);
    int m=strlen(data_s);
    int r=find_r(m);
    printf("\nNumber of redundant bits: %d",r);
    int data_len=m+r;
    printf("\nNumber of bits in code: %d",data_len);
    int digit;
    for(int i=1;i<=data_len;i++){
        digit=data%10;
        if(r_position(i)){
            code[data_len-i]=2;
        }
        else{
            code[data_len-i]=digit;
            data=data/10;
        }
    }
    /*printf("\n");
    for(int i=0;i<data_len;i++){
        printf("%d",code[i]);
    }
    printf("\n");*/
    int binary,count=0;
    for(int i = 0; i < r; i++)
    {
        for(int j = 1; j <= data_len; j++)
        {
            if((int)(pow(2, i)) != j)
            {
                binary = to_binary(j);
                //printf("%d",bin);
                if(ispresent(binary, i + 1))
                    count += code[data_len - j];
            }
            //printf("%d",j);
        }
        //check for even parity
        if(count % 2 == 0)
            code[data_len - (int)(pow(2, i))] = 0;
        else
            code[data_len - (int)(pow(2, i))] = 1;
        count = 0;
        //printf("\n%d\n",i);
    }
    printf("\nData with redundant bits: ");
    for(int i = 0; i < data_len; i++)
        printf("%d", code[i]);
    printf("\n");
    //introduce error
    int pos = rand() % data_len + 1;
    printf("\nIntroducing error automatically at bit: %d\n", pos);
    if(code[data_len - pos] == 0)
        code[data_len - pos] = 1;
    else
        code[data_len - pos] = 0;
    int k = 0;
    long num = 0;
    for(int i = data_len-1; i >= 0; i--)
    {
        num += pow(10, k) * code[i];
        k++;
    }
    sprintf(data_s, "%lu", num);
    printf("Data transmitted is %s\n", data_s);
    //stop

    len = sizeof(client_address);
    newfd = accept(sockfd, (struct sockaddr*)&client_address, &len);
    m = write(newfd, data_s, sizeof(data_s));
    close(sockfd);
}
