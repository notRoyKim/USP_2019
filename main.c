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
#include <pthread.h>

char basicpath[512] = ".";

void *threadfunc(void *vargp);

char* find_content_type (char *filename);

void refered(int ns, char* filename);
   
int main(int argc, char* argv[]) {    
	struct sockaddr_in address;
	int create_socket;
	int *new_socket;
	int optvalue = 1;
	socklen_t addrlen;

	pthread_t tid;

	int PORT = atoi(argv[2]);
	int i;

	char getPath[512];
	char str[BUFSIZ] = {};
	char buffer[1024] = {};

	strcat(basicpath,argv[1]);

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	
	setsockopt(create_socket, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	address.sin_port = htons(PORT);

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1){    
		perror("bind");
		exit(1);
	}

	if (listen(create_socket, 10) < 0)
	{    
		perror("server: listen");    
		exit(1);
	}

	while (1)
	{
		new_socket = (int *)malloc(sizeof(int));
		if ((*new_socket = accept(create_socket, (struct sockaddr *)&address, &addrlen))==-1){
			perror("accept");
			exit(1);
		}
		pthread_create(&tid, NULL, threadfunc, (void*)new_socket);
	}
	close(create_socket);
	return 0;
}

char* find_content_type (char *filename) {
	char buf1[256];
	char *buf2;
	char *token;

	strcpy(buf1, filename);

	token = strtok(buf1, ".");
	while(token != NULL)
	{
		buf2 = token;
		token = strtok(NULL, ".");
	}

	if(strcmp(buf2, "html") == 0 || strcmp(buf2, "htm") ==  0)
		strcpy(buf2, "Content-Type: text/html \r\n");
	else if(strcmp(buf2, "txt") == 0)
		strcpy(buf2, "Content-Type: text/plain \r\n");
	else if(strcmp(buf2, "jpg") == 0 || strcmp(buf2, "jpeg") ==  0)
		strcpy(buf2, "Content-Type: image/jpeg \r\n");
	else if(strcmp(buf2, "gif") == 0)
		strcpy(buf2, "Content-Type: image/gif \r\n");
	else
		strcpy(buf2, "Content-Type: application/octet-stream \r\n");

	return buf2;
}

void refered(int ns, char* filename) {
	struct stat filestat;
	FILE *fp;
	int fd;
	char header_buff[2048] = {};
	char file_buff [1000000] = {};
	char filesize[8];
	char filetype[512] = {};

	strcpy(filetype,find_content_type(filename));

	if(((fd = open(filename, O_RDONLY)) < -1) || (fstat(fd, &filestat) < 0))
	{
		printf("Error in measuring the size of the file\n");
	}
	else
		sprintf(filesize, "%zd", filestat.st_size);

	if((fp = fopen(filename,"r")) ==  NULL)
	{
		printf("can't open file\n");
				
	}

	if(fp == NULL)
	{
		strcpy (header_buff, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\n\r\n");
		write (ns, header_buff, strlen(header_buff));
	}
	else if(fp != NULL)
	{
		strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
		strcat (header_buff, filesize);
		strcat (header_buff, "\r\n");
		strcat (header_buff, filetype);
		strcat (header_buff, "Connection: keep-alive\r\n\r\n");
		fread (file_buff, sizeof(char), filestat.st_size + 1, fp);
		write (ns, header_buff, strlen(header_buff));
		write (ns, file_buff, filestat.st_size);
		fclose(fp);
	}
	close(ns);
}

void *threadfunc(void *vargp)
{
	int* new_socket = (int *) vargp;
	struct sockaddr_in address;
	socklen_t addrlen;
	char buffer[1024] = {};
	char getPath[1024] = {};

	recv(*new_socket, buffer, 1024, 0);
	printf("%s\n", buffer);

	strcpy(getPath,basicpath);
	strtok(buffer," ");
	strcat(getPath,strtok(NULL," "));
	
	refered(*new_socket,getPath);
	free(vargp);
	return NULL;
}
