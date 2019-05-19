#include<stdio.h>    
#include<stdlib.h>    
#include<sys/socket.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<unistd.h>

#include <arpa/inet.h>
#include <sys/un.h>

    
int main(int argc, char* argv[]) {    
	int create_socket, new_socket;
	int optvalue = 1;
	socklen_t addrlen;
	int bufsize = 1024;
	char *buffer = malloc(bufsize);
	struct sockaddr_in address;
	int PORT = atoi(argv[1]);
	
	FILE *fp;
	char str[BUFSIZ -1] = {};
 	
	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_port = htons(PORT);
    
	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1){    
		perror("bind");
		exit(1);
	}
    	setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));
    
	while (1) {    
	if (listen(create_socket, 10) < 0) {    
		perror("server: listen");    
		exit(1);    
	}
	if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) == -1) {    
		perror("server: accept");
		exit(1);
	}
	if (new_socket > 0){    
		printf("The Client is connected...\n");
	}
        
	recv(new_socket, buffer, bufsize, 0);
	printf("%s\n", buffer);
	
	fp = fopen("./html/index.html","r+");
	while(fgets(str,100,fp))
	{
		write(new_socket, str, strlen(str));
	}    
      	close(new_socket);    
   }
   close(create_socket);    
   return 0;    
}
