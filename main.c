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

void refered(int ns, char* filename);

void responseCGI(int ns, char * cgi);
   
int main(int argc, char* argv[]) {    
	struct sockaddr_in address;
	int create_socket;
	int *new_socket;
	int optvalue = 1;
	socklen_t addrlen;
	pthread_t tid;
	int PORT = atoi(argv[2]);

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

	if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1)
	{    
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
		pthread_join(tid,NULL);
	}
	close(create_socket);
	return 0;
}

void refered(int ns, char* filename) {
	struct stat filestat;
	FILE *fp;
	int fd;
	char header_buff[2048] = {};
	char file_buff [1000000] = {};
	char filesize[8];

	if(((fd = open(filename, O_RDONLY)) < -1) || (fstat(fd, &filestat) < 0))
	{
		printf("Error in measuring the size of the file\n");
	}
	else
	{
		sprintf(filesize, "%zd", filestat.st_size);
	}
	if((fp = fopen(filename,"r")) ==  NULL)
	{
		strcpy (header_buff, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\n\r\n");
		write (ns, header_buff, strlen(header_buff));
	}
	else
	{
		strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
		strcat (header_buff, filesize);
		strcat (header_buff, "\r\nContent-Type: */* \r\nConnection: keep-alive\r\n\r\n");
		fread (file_buff, sizeof(char), filestat.st_size + 1, fp);

		write (ns, header_buff, strlen(header_buff));
		write (ns, file_buff, filestat.st_size);
		close(fd);
		fclose(fp);


	}
}

void *threadfunc(void *vargp)
{
	int* new_socket = (int *) vargp;
	struct sockaddr_in address;
	socklen_t addrlen;
	char buffer[1024] = {};
	char getPath[512] = {};
	char openfilename[200] = {};
	FILE * fp;

//	fp = fopen("log.txt","a+");

	recv(*new_socket, buffer, 1024, 0);
	if(strcmp(buffer,"") == 0)
		return NULL;
	
	printf("%s\n",buffer);

	strcpy(getPath,basicpath);
	strtok(buffer," ");
	strcpy(openfilename,strtok(NULL," "));

	if(strcmp(openfilename,"/") == 0)
		strcpy(openfilename,"/index.html");

	if(strstr(openfilename,"total.cgi") != NULL)
	{
		responseCGI(*new_socket,openfilename);
	}
	else
	{
//		printf("get %s\n",getPath);
		strcat(getPath,openfilename);
		refered(*new_socket,getPath);
	}

	close(*new_socket);
	free(vargp);
	return NULL;
}

void responseCGI(int ns, char * cgi)
{
	int n;
	int m;
	int i;
	int sum = 0;
	char file[64] = {};
	char header_buff[2048] = {};
	char filelen[100] = {};
	int ifilelen;
	
	strtok(cgi,"=");
	n = atoi(strtok(NULL,"&"));
	strtok(NULL,"=");
	m = atoi(strtok(NULL," "));

	for(i = n; i <= m ; i++)
		sum += i;

	sprintf(file, "%d", sum);
	ifilelen = strlen(file);
	sprintf(filelen,"%d",ifilelen);
	
	strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
	strcat (header_buff,filelen);
	strcat (header_buff, "\r\nContent-Type: text/plain \r\nConnection: keep-alive\r\n\r\n");

	write (ns, header_buff, strlen(header_buff));
	write(ns, file,ifilelen);
}
