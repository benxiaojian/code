#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h> //linux 
#include <sys/socket.h> //linux 
#include <netdb.h> //linux 
#include <sys/types.h> //linux

int socket_listen(int port)
{
	int listenfd,connfd,sin_size;
	struct sockaddr_in servaddr,their_addr;
	char buffer[1024];
	
	if ((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket error\n");
		return -1;
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons((unsigned short)port); 
	
	if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
		return -1;
	//ÓÃsocket()´´½¨Ì×½Ó¿Úºó£¬Ëü±ã´æÔÚÓÚÒ»¸öÃû×Ö¿Õ¼ä£¨µØÖ·×å£©ÖÐ£¬µ«²¢Î´¸³Ãû¡£
	//bind()º¯ÊýÍ¨¹ý¸øÒ»¸öÎ´ÃüÃûÌ×½Ó¿Ú·ÖÅäÒ»¸ö±¾µØÃû×ÖÀ´ÎªÌ×½Ó¿Ú½¨Á¢±¾µØÀ¦°ó£¨Ö÷»úµØÖ·/¶Ë¿ÚºÅ£©¡£
	if (listen(listenfd,10)==-1)
	{
		printf("listen error\n");
		return -1;
	}
	while (1)
	{
		printf("listening....\n");
		connfd = accept(listenfd,(struct sockaddr *)&their_addr,&sin_size);
		printf("connect~\n");
		strcpy(buffer,"hello ben!~");
		write(connfd,buffer,strlen(buffer));
	}
	
}

int main()
{
	socket_listen(8081);
}
