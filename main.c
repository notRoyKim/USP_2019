#include<stdio.h>    
#include<stdlib.h>    
#include<sys/socket.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/sendfile.h>
#include <fcntl.h>

// char* find_get_name (char *buf)

char* find_content_type (char *filename) {
	char *p;
	int i;
	char buf1[256];
	char *buf2;
	char *token;

	p = (char *)malloc(30);
	strcpy(buf1, filename);

	token = strtok(buf1, ".");
	while(token != NULL)
	{
		buf2 = token;
		token = strtok(NULL, ".");
	}

	if(strcmp(buf2, "html") == 0 || strcmp(buf2, "hml") ==  0)
		strcpy(buf2, "Content-Type: text/html \r\n");
	else if(strcmp(buf2, "txt") == 0)
		strcpy(buf2, "Content-Type: text/plain \r\n");
	else if(strcmp(buf2, "jpg") == 0 || strcmp(buf2, "jpeg") ==  0)
		strcpy(buf2, "Content-Type: image/jpeg \r\n");
	else if(strcmp(buf2, "gif") == 0)
		strcpy(buf2, "Content-Type: image/gif \r\n");
	else
		strcpy(buf2, "Content-Type: application/octet-stream \r\n");

	p = buf2;
	printf("content-type: %s\n", p);
	return p;
}
    
int main(int argc, char* argv[]) {    
	int create_socket, new_socket;
	int optvalue = 1;
	socklen_t addrlen;
	int bufsize = 1024;
	char *buffer = malloc(bufsize);
	struct sockaddr_in address;
	int PORT = atoi(argv[1]);

	int i;
	int sendimg;
	char *path;
	char getPath[256] = "./html";

	FILE *fp;
	char str[BUFSIZ -1] = {};

	char *cont;
 	
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

// finding GET
		strtok(buffer," ");
		path = strtok(NULL," ");
		
		if(!strcmp(path,"/")){
			fp = fopen("./html/index.html","r");
			while(fgets(str,10000,fp))
			{
				write(new_socket, str, strlen(str));
			}
		}
		else{
			strcat(getPath,path);
			sendimg = open(getPath, O_RDONLY);
			sendfile(new_socket,sendimg,NULL,10000);
			close(sendimg);
		}

		close(new_socket);
	}
	close(create_socket);
	return 0;
}
