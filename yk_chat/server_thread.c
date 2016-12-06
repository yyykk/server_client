#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

char *haddrp;
struct hostent *hp;

void *readfromclient(void *vargp){//线程例程

	int connfd = *((int *)vargp);
	time_t t;  

	pthread_detach(pthread_self());
	
	while(1){
		time(&t); 
		char client_word[100];
		read(connfd, client_word, 100);
		printf("\n%s\t%s said : %s \n", ctime(&t), haddrp, client_word);
		if(!strcmp(client_word, "exit")){
			close(connfd);
			exit(0);
		}	
	}
	//close(connfd);
	return NULL;
}

void *write2client(void *vargp){//线程例程

	int connfd = *((int *)vargp);
	printf("You can chat with %s (%s)!\n", hp->h_name, haddrp);
	time_t t;	  

	pthread_detach(pthread_self());
	
	while(1){
		time(&t);
		char client_word[100];
		//printf("%s\tyou said : ", ctime(&t));
		gets(client_word);
		write(connfd, client_word, 100);
		if(!strcmp(client_word, "exit")){
			close(connfd);
			exit(0);
		}	
	}
	//close(connfd);
	return NULL;
}


int main(int argc, char **argv){
	int port, listenfd, clientlen;	
	struct sockaddr_in clientadder;
	
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
	
		printf("\033c");
		clientlen = sizeof(clientadder);
		int *connfd = malloc(sizeof(int));//由于线程间容易产生竞争，所以必须给每个线程的连接描述符分配独立的内存空间
		if((*connfd = accept(listenfd, (struct sockaddr *)&clientadder, &clientlen)) < 0){
			printf( "accept error\n" );
		}else{
			hp = gethostbyaddr((const char *)&clientadder.sin_addr.s_addr, 
					sizeof(clientadder.sin_addr.s_addr), 
						AF_INET);
			haddrp = inet_ntoa(clientadder.sin_addr);
			pthread_t this_id;	
			pthread_create(&this_id, NULL, readfromclient, connfd);//创建一个线程，跳向线程例程
			pthread_create(&this_id, NULL, write2client, connfd);//创建一个线程，跳向线程例程
		}		
	while(1);
}

