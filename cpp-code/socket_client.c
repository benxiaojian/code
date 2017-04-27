#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h> //linux 
#include <sys/socket.h> //linux 
#include <netdb.h> //linux 
#include <sys/types.h> //linux
//#include <winsock2.h>

//创建一个socket连接并返回 
int socket_connect(char *host,int port)
{
	struct sockaddr_in address; //定义地址结构
	int sockfd; //socket的标识 
	int opvalue;
	struct hostent *hostent;
	//socklen_t slen;
	//opvalue = 8;
	//slen = sizeof(opvalue);
	hostent = gethostbyname(host);
	printf("1\n");
	if (!hostent)
		return -1;
	memset(&address,0,sizeof(address));
	printf("%s\n",host);
	if (sockfd = socket(AF_INET,SOCK_STREAM,0) < 0)
		return -1;
	//获得socket的标识 
	
	//设置接收和发送超时
	struct timeval timeo = {15,0};
	setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeo,sizeof(timeo));
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeo,sizeof(timeo));
	
	address.sin_family = AF_INET; //设地址族 
	address.sin_port = htons((unsigned short)port); //设端口号
	address.sin_addr = *((struct in_addr*)hostent->h_addr);
	//struct hostent* server = gethostbyname(host);
	// gethostbyname 返回对应于给定主机名的包含主机名字和地址信息的hostent结构指针
	
//	printf("1\n");
//	if (!server)
//		return -1;
//	
//	memcpy(&address.sin_addr.s_addr,server->h_addr_list,server->h_length);
	
	//inet_pton(AF_INET,host,&address.sin_addr);
	

	//printf("2\n");	
	if (connect(sockfd,(struct sockaddr *)&address,sizeof(address)) == -1)
	{
		printf("connect fail\n");
		return -1;
	}
	return sockfd;
}




int main(int argc,char *argv[])
{
	int sockfd;
	char recvline[1024];
	int n;
	sockfd = socket_connect("182.254.226.222",8081);
	if (sockfd == -1) printf("failed\n");
	//if (recv(sockfd,recvline,1024,0) != -1)
	//	printf("%s\n",recvline);
	
	while (1)
	{
	send(sockfd,"hello",sizeof("hello"),0);
	send(sockfd,"world",sizeof("world"),0);
	char str[128];
	memset(str,0,sizeof(str));
	scanf("%s",str);
	send(sockfd,str,sizeof(str),0);
	}
}
