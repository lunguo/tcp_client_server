#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int sock_server;

int main()
{
    /*if(argc!=3)
    {
        printf("参数错误!\n");
        exit(1);
    }*/

    //创建套接字
    sock_server=socket(AF_INET, SOCK_STREAM, 0);

    int optval=1;
    setsockopt(sock_server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    
    //绑定地址
    struct sockaddr_in myaddr;
    myaddr.sin_family=AF_INET;
    myaddr.sin_addr.s_addr=INADDR_ANY;
    //myaddr.sin_addr.s_addr=inet_addr("120.79.206.182");
    myaddr.sin_port=htons(9999);

    if(bind(sock_server, (struct sockaddr*)&myaddr, sizeof(myaddr))==-1)
    {
        perror("bind error");
        exit(1);
    }

        printf("正在等待连接...\n");    
    //设置监听状态
    listen(sock_server, 5);

    struct sockaddr_in addr_client;
    socklen_t len;

    //接受客户连接请求
        int sock_conn;
        len=sizeof(addr_client);
        sock_conn=accept(sock_server, (struct sockaddr*)&addr_client, &len);
            

        if(sock_conn!=-1)
        {
            printf("\n客户端%s:%d已经连接!\n", inet_ntoa(addr_client.sin_addr), ntohs(addr_client.sin_port));
            
            int pid=fork();

            if(pid>0)
            {
                while(1)
                {
                    char msg[1025];
                    int ret;

                    sleep(1);
                    ret=recv(sock_conn, msg, sizeof(msg)-1, 0);

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
                    char mes[1025];
                    scanf("%s", mes);
                    printf("我说：%s\n", mes);
                    send(sock_conn, mes, strlen(mes), 0);
                }
            }
        }
        close(sock_conn);

    close(sock_server);
    
    return 0;
}

