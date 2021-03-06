#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void remove_zombie(int sig);

int main()
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    int state;

    socklen_t clnt_adr_sz;
    int str_len;
    char buf[BUF_SIZE];

    //signal handling 등록
    act.sa_handler = remove_zombie;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);

    //socket 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(serv_sock == -1)
        error_handling("server socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(58700);

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while(1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        
        if(clnt_sock==-1)
            continue;
        
        pid = fork();

        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }

        if(pid == 0) //자식 프로세스 
        {
            close(serv_sock);

            while( (str_len=read(clnt_sock, buf, BUF_SIZE)) != 0 )
            {
                //second client로 보내기
                
            }
            
            close(clnt_sock);
            puts("client disconnected!");
            return 0;
        }
        else
        {
            close(clnt_sock);
        }
    }

    close(serv_sock);
    return 0;
}

void remove_zombie(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed pid: %d\n", pid);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
