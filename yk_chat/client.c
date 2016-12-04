#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void *readfromserver(void *vargp){//线程例程

	int connfd = *((int *)vargp);
	time_t t;  

	pthread_detach(pthread_self());
	
	while(1){
		time(&t);
		char client_word[100];
		read(connfd, client_word, 100);
		printf("\n%s\tyour friend said : %s \n", ctime(&t), client_word);
		if(!strcmp(client_word, "exit")){
			close(connfd);
			exit(0);
		}	
	}
	//close(connfd);
	return NULL;
}

void *write2server(void *vargp){//线程例程
	
	int connfd = *((int *)vargp);
	time_t t;  

	pthread_detach(pthread_self());
	
	while(1){
		time(&t);
		char client_word[100];
		
		gets(client_word);
		write(connfd, client_word, 100);
		if(!strcmp(client_word, "exit")){
			close(connfd);
			exit(0);
		}
	}
	
	return NULL;
}




int main(int argc, char **argv){
	int port,clientfd;
	struct hostent *hp;
	char *EXIT = "exit is getted";
	struct sockaddr_in serveraddr;
	
	port = atoi(argv[2]);

	if((hp = gethostbyname(argv[1])) == NULL){
		printf("ip error\n");
	}

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket error\n");
	} 

	bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	
	if(connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
		printf("connect error\n");
	}else{
		pthread_t this_id;
		printf("\033c");		
		printf("You can chat with %d!\n", clientfd);
		pthread_create(&this_id, NULL, readfromserver, &clientfd);//创建一个线程，跳向线程例程
		pthread_create(&this_id, NULL, write2server, &clientfd);//创建一个线程，跳向线程例程
		while(1);
	}
	return 0;
}
