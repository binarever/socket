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

void checkcmd(char* recvbuf,char* sendbuf)
{
	int flag;
	if(recvbuf[0]=='c'&&recvbuf[1]=='r'&&recvbuf[2]=='e'&&recvbuf[3]=='a'&&recvbuf[4]=='t'&&recvbuf[5]=='e'&&recvbuf[6]==' ')
		flag=1;
	if(!strcmp(recvbuf,"help\n")||strcmp(recvbuf,"create\n")>=0)
	{
		if(!strcmp(recvbuf,"help\n"))
		{
			char buffer[]="[*] create <filename>\n[*] date\n";
			memcpy(sendbuf,buffer,sizeof(buffer));
			return;
		}
		if(!strcmp(recvbuf,"create\n"))
		{
			char buffer[]="[-] createfileusage:create <filename>\n";
			memcpy(sendbuf,buffer,sizeof(buffer));
			return;
		}
		if(strlen(recvbuf)>8&&flag)
		{
			char *filename=&recvbuf[7];
			char op[64]="mkdir ";
			strcat(op,filename);
			if(!system(op))
			{
				char buffer[]="[+] create file successfully\n";
				memcpy(sendbuf,buffer,sizeof(buffer));
				return;
			}
			else
			{
				char buffer[]="[-] Sorry, create file failed\n";
				memcpy(sendbuf,buffer,sizeof(buffer));
				return;
			}

		}
		memcpy(sendbuf,recvbuf,sizeof(recvbuf));
	}
	else
	{
		memcpy(sendbuf,recvbuf,sizeof(recvbuf));
		return;
	}
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
    int flag=0;
    //主线程
    while(1) {

        memset(recvbuf, 0 ,sizeof(recvbuf));
        int len = recv(conn, recvbuf, sizeof(recvbuf), 0);
        if(strcmp(recvbuf, "exit\n") == 0) break;
	if(recvbuf[0]=='c'&&recvbuf[1]=='r'&&recvbuf[2]=='e'&&recvbuf[3]=='a'&&recvbuf[4]=='t'&&recvbuf[5]=='e'&&recvbuf[6]==' ')
		flag=1;

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

	if(!strcmp(recvbuf,"help\n"))
	{
		char buffer[]="[*] create <filename>\n[*] date\n";
		memcpy(sendbuf,buffer,sizeof(buffer));	
	}
	if(!strcmp(recvbuf,"create\n"))
	{
		char buffer[]="[-] createfileusage:create <filename>\n";
		memcpy(sendbuf,buffer,sizeof(buffer));
	}
	if(strlen(recvbuf)>8&&flag)
	{
		char *filename=&recvbuf[7];
		char op[64]="mkdir ";
		strcat(op,filename);
		if(!system(op))
		{
			char buffer[]="[+] create file successfully\n";
			memcpy(sendbuf,buffer,sizeof(buffer));
		}
		else
		{
			char buffer[]="[-] Sorry, create file failed\n";
			memcpy(sendbuf,buffer,sizeof(buffer));
		}

	}
	if(strcmp(recvbuf,"date\n")&&strcmp(recvbuf,"help\n")&&!flag&&strcmp(recvbuf,"create\n"))
	{
		memcpy(sendbuf,recvbuf,sizeof(recvbuf));
	}

	//checkcmd(recvbuf,sendbuf);
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
