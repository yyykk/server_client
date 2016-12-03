#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int windows_max = 250;
int windows_now = 250;

int echo(int connfd){
	int sure_word = 0;	
	while(1){
		char num[100];
		windows_now = rand() % windows_max;
		sprintf(num, "%d",  windows_now);
		printf("\n");
		printf("send a new windows = %d\n", windows_now);

		sleep(1);
		write(connfd, num, 100);

		while(windows_now != 0){
			char client_word[100];

			read(connfd, client_word, 100);
			if(!strcmp(client_word, "exit")){
				printf("exit\n");
				return 1;
			}
			windows_now -= atoi(client_word);
			sure_word += atoi(client_word);
			sleep(0.8);
			printf("sure of %d\n", sure_word);
		}
		windows_now = windows_max;
	}
	printf("end\n"); 
	return 1; 
}

void *client(void *vargp){
	int connfd = *((int *)vargp);
	pthread_detach(pthread_self());
	printf("\033c");
	if(echo(connfd)){
		printf("exit from client\n");
	}else{
		printf("error\n");
	}

	free((int *)vargp);
	close(connfd);

	return NULL;
}

int main(int argc, char **argv){
	int port, listenfd, connfd, clientlen;
	struct sockaddr_in clientadder;
	struct hostent *hp;
	port = atoi(argv[1]);
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf( "socket error\n" );
		exit(0);
	}

	clientadder.sin_family = AF_INET;
	clientadder.sin_addr.s_addr = htonl(INADDR_ANY);
	clientadder.sin_port = htons(port); 

	if(bind(listenfd, (struct sockaddr *)&clientadder, sizeof(clientadder)) < 0){
		printf( "bind error\n" );
		exit(0);
	}

	if (listen(listenfd, 10) < 0){
		printf( "listen error\n" );
		exit(0);
	}
	
	while(1){
		clientlen = sizeof(clientadder);
		int *connfd = malloc(sizeof(int));//由于线程间容易产生竞争，所以必须给每个线程的连接描述符分配独立的内存空间
		printf("\033c");		
		printf("wait for client\n");		
		if((*connfd = accept(listenfd, (struct sockaddr *)&clientadder, &clientlen)) < 0){
			printf( "accept error\n" );
			exit(0);
		}else{			
			pthread_t this_id;	
			pthread_create(&this_id, NULL, client, connfd);//创建一个线程，跳向线程例程
		}		
	}
	printf("gg\n");
}

