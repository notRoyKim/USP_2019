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

typedef struct SocketSet
{
	int ns;
	char* ip;
}SocketSet;

void *threadfunc(void *vargp);

void refered(SocketSet* set_socket, char* filename);

void responseCGI(SocketSet* set_socket, char * cgi);

void logwrite();

void filewrite(char *ip, char *filename, int size);
   
int main(int argc, char* argv[]) {
	int create_socket;
	int optvalue = 1;
	int PORT = atoi(argv[2]);
	struct sockaddr_in address, client;
	socklen_t addrlen, clilen;
	struct SocketSet * ipAndSd;
	pthread_t tid;
	char aa[20] = {};
	FILE *fp;

	fp = fopen("log.txt","w");
	fclose(fp);
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
		ipAndSd = (SocketSet *) malloc(sizeof(SocketSet));

		if ((ipAndSd->ns = accept(create_socket, (struct sockaddr *)&client, &clilen))==-1)
		{
			perror("accept");
			exit(1);
		}
		ipAndSd->ip = inet_ntoa(client.sin_addr);

		pthread_create(&tid, NULL, threadfunc, (void*)ipAndSd);
		pthread_join(tid,NULL);
	}
	close(create_socket);
	return 0;
}

void refered(SocketSet* set_socket, char* filename) {
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
		write (set_socket->ns, header_buff, strlen(header_buff));
		filewrite(set_socket->ip, filename, 0);
	}
	else
	{
		strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
		strcat (header_buff, filesize);
		strcat (header_buff, "\r\nContent-Type: */* \r\nConnection: keep-alive\r\n\r\n");
		fread (file_buff, sizeof(char), filestat.st_size + 1, fp);
		write (set_socket->ns, header_buff, strlen(header_buff));
		write (set_socket->ns, file_buff, filestat.st_size);
		close(fd);
		fclose(fp);
		filewrite(set_socket->ip, filename, filestat.st_size);
	}
}

void *threadfunc(void *vargp)
{
	struct SocketSet* set_socket = (SocketSet *) vargp;
	struct sockaddr_in address;
	socklen_t addrlen;
	char buffer[1024] = {};
	char getPath[512] = {};
	char openfilename[200] = {};
	FILE * fp;

	recv(set_socket->ns, buffer, 1024, 0);
	if(strcmp(buffer,"") == 0)
		return NULL;
	
	printf("%s\n",buffer);

	strcpy(getPath,basicpath);
	strtok(buffer," ");
	strcpy(openfilename,strtok(NULL," "));

	if(strcmp(openfilename,"/") == 0)
		strcpy(openfilename,"/index.html");
	strcat(getPath,openfilename);

	if(strstr(openfilename,"total.cgi") != NULL)
		responseCGI(set_socket,openfilename);
	else
		refered(set_socket,getPath);

	close(set_socket->ns);
	free(vargp);
	return NULL;
}

void responseCGI(SocketSet* set_socket, char * cgi)
{
	int n, m, i, ifilelen;
	int sum = 0;
	char file[64] = {};
	char header_buff[2048] = {};
	char filelen[100] = {};
	char operation[100] = {};

	strcpy(operation,cgi);
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

	write (set_socket->ns, header_buff, strlen(header_buff));
	write(set_socket->ns, file,ifilelen);
	filewrite(set_socket->ip, operation, ifilelen);
}

void filewrite(char *ip, char *filename, int size)
{
	FILE *fp;
	
	fp = fopen("log.txt","a+");
	fprintf(fp, "%s %s %d\n",ip,filename,size);
	
	fclose(fp);
}
