#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int echo(int connfd){
	char *exit = "exit is getted";
	write(connfd, "Please input", 100);	
	while(1){
		char client_word[100];
		read(connfd, client_word, 100);
		printf("%s \n", client_word);
		strcat(client_word, " is getted");
		write(connfd, client_word, 100);
		if(strcmp(client_word, exit) == 0){
			printf("client exit\n");
			return 1;
		}else{
			continue;
		}
	} 
}

int main(int argc, char **argv){
	int port, listenfd, connfd, clientlen;
	struct sockaddr_in clientadder;
	struct hostent *hp;
	port = atoi(argv[1]);

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf( "socket error\n" );
	}

	clientadder.sin_family = AF_INET;
	clientadder.sin_addr.s_addr = htonl(INADDR_ANY);
	clientadder.sin_port = htons(port); 

	if(bind(listenfd, (struct sockaddr *)&clientadder, sizeof(clientadder)) < 0){
		printf( "bind error\n" );
	}

	if (listen(listenfd, 10) < 0){
		printf( "listen error\n" );
	}
	
	while(1){
		printf( "wait for client\n" );
		clientlen = sizeof(clientadder);
		if((connfd = accept(listenfd, (struct sockaddr *)&clientadder, &clientlen)) < 0){//父进程创建连接描述符
			printf( "accept error\n" );
		}else{
			if(fork() == 0){
				close(listenfd);//子进程关闭来自父进程的监听描述符
				printf("connect successful!\n");
				if (echo(connfd)){
					printf("exit from echo\n");	
				}else{
					printf("error from echo\n");
				}
				write(connfd, "exit", 100);
				close(connfd);//关闭父进程送进来的连接描述符
				exit(0);
			}
			close(connfd);//父进程关闭已被子进程复制的连接描述符		
		}			
	}
	printf("gg\n");
}

