#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int sock;

int main(int argc, char** argv)
{
    if(argc!=3)
    {
        printf("参数错误!\n");
        exit(1);
    }

    sock=socket(AF_INET, SOCK_STREAM, 0);

    /*struct sockaddr_in addr_client;
    addr_client.sin_family=AF_INET;
    addr_client.sin_addr.s_addr="123.207.157.91";
    addr_client.sin_port=htons(8888);*/

    struct sockaddr_in addr_server;
    addr_server.sin_family=AF_INET;
    addr_server.sin_addr.s_addr=inet_addr(argv[1]);
    addr_server.sin_port=htons(atoi(argv[2]));
    
    printf("尝试连接...\n");
    while(connect(sock, (struct sockaddr*)&addr_server, sizeof(addr_server))==-1)
    {
        perror("connect error");
        printf("正在重新连接...\n");
        sleep(2);
    }
    printf("连接成功!\n");
    
    int pid=fork();
    if(pid>0)
    {
        while(1)
        {
            char msg[1025];
            int ret;

            ret=recv(sock, msg, sizeof(msg)-1, 0);
            if(ret>0)
            {
                msg[ret]='\0';
                printf("对方说：%s\n", msg);
            }
        }
    }
    else
    {
        while(1)
        {
            char mes[1024];
            scanf("%s", mes);
            printf("我说：%s\n", mes);
            send(sock, mes, strlen(mes), 0);
        }
    }
    close(sock);

    return 0;
}
