#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int word = 500;
int windows_now = 0;
int tosend = 50;

int main(int argc, char **argv){
	int port,clientfd;
	struct hostent *hp;

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
		printf("\033c");
		printf("connect successful!\n");
		while(1){
			char buf[100];
			char get[100];
			
			read(clientfd, get, 100);
			
			windows_now += atoi(get);
			printf("\n");
			printf("get new windows_now = %d \n", windows_now);
			while(windows_now != 0){
				tosend = rand() % 51;
				if(tosend > windows_now){
					tosend = windows_now;
				}
				if(tosend > word){
					tosend = word;
				}
				sprintf(buf, "%d",  tosend);
				word -= tosend;
				windows_now -= tosend;
				if(word != 0){
					sleep(1);
					write(clientfd, buf, 100);
					printf("send %s to server, windows_now = %d, unsend = %d\n"
						, buf, windows_now, word);
				}else{
					write(clientfd, buf, 100);
					printf("send %s to server, windows_now = %d, unsend = %d\n"
						, buf, windows_now, word);		
					write(clientfd, "exit", 100);
					printf("send all\n");
					return 0;
				}
			}
		} 

	}
	return 0;
}
