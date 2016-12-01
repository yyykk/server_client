#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

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

void *thread(void *vargp){//线程例程

	printf("connect successful!\n");
	int connfd2 = *((int *)vargp);

	pthread_detach(pthread_self());
	
	//printf("connfd2 = %d \n",connfd2);
	if (echo(connfd2)){
		printf("exit from echo\n");	
	}else{
		printf("error from echo\n");
	}
	write(connfd2, "exit", 100);

	free((int *)vargp);
	close(connfd2);

	return NULL;
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
		int *connfd = malloc(sizeof(int));//由于线程间容易产生竞争，所以必须给每个线程的连接描述符分配独立的内存空间
		if((*connfd = accept(listenfd, (struct sockaddr *)&clientadder, &clientlen)) < 0){
			printf( "accept error\n" );
		}else{
			pthread_t this_id;
			printf("connfd = %d \n",*connfd);	
			pthread_create(&this_id, NULL, thread, connfd);//创建一个线程，跳向线程例程
		}		
	}
	printf("gg\n");
}

