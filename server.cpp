#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>
#define PORT 7000
#define QUEUE 20
int conn;
void thread_task() {
}

int main() {
    //printf("%d\n",AF_INET);
    //printf("%d\n",SOCK_STREAM);
    int ss = socket(AF_INET, SOCK_STREAM, 0);
    //printf("%d\n",ss);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);
    //printf("%d\n",INADDR_ANY);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1) {
        perror("bind");
        exit(1);
    }
    if(listen(ss, QUEUE) == -1) {
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    ///成功返回非负描述字，出错返回-1
    conn = accept(ss, (struct sockaddr*)&client_addr, &length);
    if( conn < 0 ) {
        perror("connect");
        exit(1);
    }

    char recvbuf[1024];
    //创建另外一个线程
    //std::thread t(thread_task);
    //t.join();
    char sendbuf[1024];
    //主线程
    while(1) {

        memset(recvbuf, 0 ,sizeof(recvbuf));
        int len = recv(conn, recvbuf, sizeof(recvbuf), 0);
        if(strcmp(recvbuf, "exit\n") == 0) break;
        printf("%s", recvbuf);

	if(!strcmp(recvbuf,"date\n"))
	{
		FILE *fp;
		fp=popen("date","r");
		if(fp==NULL)
		{
			printf("ERROR");
		}
		while(fgets(sendbuf,sizeof(sendbuf)-1,fp)!=NULL);
		fclose(fp);
	}
        //必须要有返回数据， 这样才算一个完整的请求
	else
	{
		memcpy(sendbuf,recvbuf,sizeof(recvbuf));
	}
	send(conn,sendbuf,sizeof(sendbuf),0);
	//memset(sendbuf,0,sizeof(sendbuf));
	//if(fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL)
	//{
	//	send(conn,sendbuf,sizeof(sendbuf),0);
	//}
    }
    close(conn);
    close(ss);
    return 0;
}
