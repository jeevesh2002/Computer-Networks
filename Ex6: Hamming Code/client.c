#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<math.h>

#define MAXLINE 1024

int countbits(long num)
{
    int r, count = 0;
    while(num > 0)
    {
        num = num / 10;
        count++;
    }
    return count;
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

int decimal(int num)
{
    int rem, i = 0, result;
    while(num > 0)
    {
        rem = num % 10;
        result += pow(2, i) * rem;
        num /= 10;
        i++;
    }
    return result;
}

int main(int argc, char **argv)
{
    int PORT = 8080;

    long num;
    int sockfd, data_len,rem, code[40], count = 0, r = 0, result = 0,newarr[20], finalarr[20];
    char buff[40],data_s[50];
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed!");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int n, len;
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    n = read(sockfd, buff, sizeof(buff));
    num = atol(buff);
    sprintf(data_s,"%lu",num);
    data_len=strlen(data_s);
    printf("Received data: %lu\n", num);
    int i = 1;
    while(num > 0)
    {
        rem = num % 10;
        code[data_len - i] = rem;
        num /= 10;
        i++;
    }
    for(int i = 1; i <= data_len; i++)
        if(ceil(log2(i)) == floor(log2(i)))
            r++;
    int k = 0,binary ;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 1; j <= data_len; j++)
        {
            binary = to_binary(j);
            if(ispresent(binary, i + 1))
                count += code[data_len - j];
        }
        if(count % 2 == 0)
            result += pow(10, k) * 0;
        else
            result += pow(10, k) * 1;
        k++;
        count=0;
    }
    int error = decimal(result);
    printf("\nError bit in binary: %d\n", result);
    printf("\nError in bit %d\n", error);
    if(code[data_len - error] == 0)
        code[data_len - error] = 1;
    else
        code[data_len - error] = 0;
    k = 0;
    printf("\nData after error correction: ");
    for(int i = data_len - 1; i >= 0; i--)
    {
        newarr[k] = code[i];
        k++;
    }
    int x = 0;
    for(int i = 0;i < k; i++)
    {
        if(ceil(log2(i + 1)) != floor(log2(i + 1)))
        {
            finalarr[x] = newarr[i];
            x++;
        }
    }
    for(int i = x - 1; i >= 0; i--)
        printf("%d", finalarr[i]);
    printf("\n");
    return 0;
}
