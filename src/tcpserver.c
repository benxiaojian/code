#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/types.h>

#define MAX_LEN 1024
char buff[MAX_LEN];
int tcpserver_connect(int port)
{
	int sockfd;
	struct sockaddr_in address;

	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		printf("socket failed\n");
		return -1;
	}
	
	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons((unsigned short)port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd,(struct sockaddr *)&address,sizeof(struct sockaddr)) == -1)
	{
		printf("bind failed\n");
		return -1;
	}
	
	listen(sockfd,10);
	
	return sockfd;
}

int tcpserver_listen(int sockfd)
{
	int listenfd;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	while (1)
	{
		printf("listening...\n");
		listenfd = accept(sockfd,(struct sockaddr *)NULL,NULL);
		printf("connecting...\n");
		//getchar();
		//strcpy(buff,"hello ben~\n");
		char buf[1024];
		//int len = recvfrom(sockfd,buf,sizeof(buf), 0, &addr, &addr_len );
		recv(sockfd,buf,1024,0);
		printf("%s\n",buf);
		//write(listenfd,buff,strlen(buff));
	}
}

int main()
{
	int sockfd;
	sockfd = tcpserver_connect(8081);
	if (sockfd == -1)
		return;
	tcpserver_listen(sockfd);
}
