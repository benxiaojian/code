#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

#define MAX_LEN 1024

int tcpclient_connect(const char* host,int port)
{
	struct sockaddr_in address;
	int sockfd;
	struct hostent *he;

	if ((he = gethostbyname(host)) == NULL)
	{
		printf("get host by name failed\n");
		return -1;
	}

	memset(&address,0,sizeof(address));

	address.sin_family = AF_INET;
	address.sin_port = htons((unsigned short)port);
	address.sin_addr = *((struct in_addr*)he->h_addr);
	
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		printf("socket failed\n");
		return -1;
	}

	if (connect(sockfd,(struct sockaddr *)&address,sizeof(struct sockaddr))== -1)
	{
		printf("connect failed\n");
		return -1;
	}

	return sockfd;

}

int tcpclient_recv(int sockfd,char *recvline)
{
	//recv(sockfd,recvline,MAX_LEN,0);
	fd_set fd;
	int maxfd;
	
	FD_ZERO(&fd);
	FD_SET(sockfd,&fd);
	
	maxfd = sockfd++;
	while (1)
	{
		printf("!!!!\n");
		if (select(maxfd,&fd,NULL,NULL,0) > 0)
		{
			if (FD_ISSET(sockfd,&fd))
			{
				printf("????\n");
				recv(sockfd,recvline,MAX_LEN,0);
				return 1;
			}
			else 
			{
				printf("isset error\n");
					return -1;
			}
		}
		else
		{
			printf("select error\n");
			return -1;
		}
	
	}
	
	
	
	
}

int main()
{
	char recvline[MAX_LEN];
	int sockfd;
	if ((sockfd = tcpclient_connect("127.0.0.1",8080)) == -1)
		return;
	if (tcpclient_recv(sockfd,recvline)==1)
	{
		printf("%s",recvline);
	}
	
	printf("end\n");
	
}
