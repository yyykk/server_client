#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
		printf("connect successful!\n");
		while(1){
			char buf[100];
			char get[100];
			read(clientfd, get, 100);
			if(strcmp(get, EXIT) == 0){
				printf("server exit\n");
				close(clientfd);
				return 0;
			}
			printf("%s \n", get);
			scanf("%s", buf);
			write(clientfd, buf, 100);
		}
	}
	return 0;
}
