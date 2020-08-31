#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

void error_handling(char *message);

int main()
{
    int sock;
    struct sockaddr_in6 serv_addr; //IPv6 socket 구조체
    
    char message[BUF_SIZE];
    
    int str_len;
    char serv_ipaddr[INET6_ADDRSTRLEN];
    int serv_port;
    
    //socket open. 서버로 연결할 소켓 생성
    sock = socket(AF_INET6, SOCK_STREAM, 0);
    
    if(sock == -1)
        error_handling("sock() error");
    
    //IPv6용 구조체에 주소 정보 할당
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_flowinfo = 0;
    char *ip = "2001:0:53aa:64c:1861:78db:8c6e:5548";
    inet_pton(AF_INET6, ip, &serv_addr.sin6_addr); //binary 형태의 주소로 변환
    serv_addr.sin6_port = htons(50000);
    
    //connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    
    for(int i=0; i<5; i++)
    {
        char data[BUF_SIZE];

        //send data
        fgets(data, BUF_SIZE, stdin);
        str_len = send(sock, data, sizeof(data), 0);
        
        if(str_len == -1)
            error_handling("send() error");
        
        //receive data from server
        char buf[BUF_SIZE];
        str_len = recv(sock, buf, BUF_SIZE, 0);
        printf("Received Data: %s\n", buf);
    }
    
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
