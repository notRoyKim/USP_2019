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

char htmshop[4431] = {};
char pathshop[512] = {};

char htmmain[9857] = {};
char pathmain[512] = {};

char htmlinks[4472] = {};
char pathlinks[512] = {};

char htmishop[6694] = {};
char pathishop[512] = {};

char htmindex[4132] = {};
char pathindex[512] = {};

char htmilinks[3955] = {};
char pathilinks[512] = {};

char htmicontact[2476] = {};
char pathicontact[512] = {};

char htmcontact[4474] = {};
char pathcontact[512] = {};

char htmhome[4470] = {};
char pathhome[512] = {};

char jpgbullet3[766] = {};
char pathbullet3[512] = {};

char jpgbullet2[792] = {};
char pathbullet2[512] = {};

char jpgbullet1[802] = {};
char pathbullet1[512] = {};

char jpgbanner3[18167] = {};
char pathbanner3[512] = {};

char jpgbanner2[8612] = {};
char pathbanner2[512] = {};

char jpgbanner1[12563] = {};
char pathbanner1[512] = {};

char jpgbac_04[64043] = {};
char pathbac_04[512] = {};

char gif0501[16657] = {};
char path0501[512] = {};

char gif0502[150943] = {};
char path0502[512] = {};

char gif0503[24764] = {};
char path0503[512] = {};

char gif0504[64043] = {};
char path0504[512] = {};

char overgif0504[8376] = {};
char pathover0504[512] = {};

char overgif05040507[8113] = {};
char pathover05040507[512] = {};

char gif0505[13968] = {};
char path0505[512] = {};

char gif0506[8618] = {};
char path0506[512] = {};

char overgif0506[8494] = {};
char pathover0506[512] = {};

char gif0507[8514] = {};
char path0507[512] = {};

char overgif0507[8453] = {};
char pathover0507[512] = {};

char gif0508[9510] = {};
char path0508[512] = {};

char overgif0508[9325] = {};
char pathover0508[512] = {};

char gif0509[119545] = {};
char path0509[512] = {};

char gif0510[1391] = {};
char path0510[512] = {};

pthread_mutex_t mutex_lock;

typedef struct SocketSet
{
	int ns;
	char* ip;
}SocketSet;

void pre_load1();
char* pre_load3(char *filename);
void *threadfunc(void *vargp);
void refered(SocketSet* set_socket, char* filename);
void responseCGI(SocketSet* set_socket, char * cgi);
void filewrite(char *ip, char *filename, int size);
   
int main(int argc, char* argv[]) {
	int create_socket;
	int optvalue = 1;
	int PORT = atoi(argv[2]);
	struct sockaddr_in address, client;
	socklen_t addrlen, clilen;
	struct SocketSet * ipAndSd;
	pthread_t* tid;
	char aa[20] = {};
	FILE *fp;
	
	pthread_mutex_init(&mutex_lock, NULL);

	fp = fopen("log.txt","w");
	fclose(fp);

	strcat(basicpath,argv[1]);
	pre_load1();

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
		tid = (pthread_t *) malloc(sizeof(pthread_t));

		if ((ipAndSd->ns = accept(create_socket, (struct sockaddr *)&client, &clilen))==-1)
		{
			perror("accept");
			exit(1);
		}
		ipAndSd->ip = inet_ntoa(client.sin_addr);

		pthread_create(tid, NULL, threadfunc, (void*)ipAndSd);
		pthread_join(*tid,NULL);
		free(tid);
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
		strcpy (header_buff, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\n\r\n");
		write (set_socket->ns, header_buff, strlen(header_buff));
		filewrite(set_socket->ip, filename, 0);
	}
	else
	{
		sprintf(filesize, "%zd", filestat.st_size);
		strcpy (header_buff, "HTTP/1.1 200 OK\r\nContent-Length: ");
		strcat (header_buff, filesize);
		strcat (header_buff, "\r\nContent-Type: */* \r\nConnection: keep-alive\r\n\r\n");
		write (set_socket->ns, header_buff, strlen(header_buff));
		close(fd);

		if(strcmp(filename,path0501) == 0)
		{
			write (set_socket->ns, gif0501, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathindex) == 0)
		{
			write (set_socket->ns, htmindex, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathshop) == 0)
		{
			write (set_socket->ns, htmshop, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathmain) == 0)
		{
			write (set_socket->ns, htmmain, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathlinks) == 0)
		{
			write (set_socket->ns, htmlinks, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathishop) == 0)
		{
			write (set_socket->ns, htmishop, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathilinks) == 0)
		{
			write (set_socket->ns, htmilinks, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathicontact) == 0)
		{
			write (set_socket->ns, htmicontact, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathcontact) == 0)
		{
			write (set_socket->ns, htmcontact, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathhome) == 0)
		{
			write (set_socket->ns, htmhome, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbullet3) == 0)
		{
			write (set_socket->ns, jpgbullet3, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbullet2) == 0)
		{
			write (set_socket->ns, jpgbullet2, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbullet1) == 0)
		{
			write (set_socket->ns, jpgbullet1, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbanner3) == 0)
		{
			write (set_socket->ns, jpgbanner3, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbanner2) == 0)
		{
			write (set_socket->ns, jpgbanner2, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbanner1) == 0)
		{
			write (set_socket->ns, jpgbanner1, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathbac_04) == 0)
		{
			write (set_socket->ns, jpgbac_04, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0502) == 0)
		{
			write (set_socket->ns, gif0502, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0503) == 0)
		{
			write (set_socket->ns, gif0503, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0504) == 0)
		{
			write (set_socket->ns, gif0504, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathover0504) == 0)
		{
			write (set_socket->ns, overgif0504, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathover05040507) == 0)
		{
			write (set_socket->ns, overgif05040507, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0505) == 0)
		{
			write (set_socket->ns, gif0505, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0506) == 0)
		{
			write (set_socket->ns, gif0506, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathover0506) == 0)
		{
			write (set_socket->ns, overgif0506, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0507) == 0)
		{
			write (set_socket->ns, gif0507, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathover0507) == 0)
		{
			write (set_socket->ns, overgif0507, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0508) == 0)
		{
			write (set_socket->ns, gif0508, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,pathover0508) == 0)
		{
			write (set_socket->ns, overgif0508, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0509) == 0)
		{
			write (set_socket->ns, gif0509, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
		else if(strcmp(filename,path0510) == 0)
		{
			write (set_socket->ns, gif0510, filestat.st_size);
			filewrite(set_socket->ip, filename, filestat.st_size);
		}
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
	pthread_mutex_lock(&mutex_lock);
	fp = fopen("log.txt","a+");
	fprintf(fp, "%s %s %d\n",ip,filename,size);
	fclose(fp);
	pthread_mutex_unlock(&mutex_lock);
}

void pre_load1()
{
	FILE *fp;

	strcpy(pathshop, pre_load3("/shop.htm"));
	fp = fopen(pathshop,"r");
	fread (htmshop, sizeof(char), 4431, fp);
	fclose(fp);

	strcpy(pathmain, pre_load3("/main.htm"));
	fp = fopen(pathmain,"r");
	fread (htmmain, sizeof(char), 9857, fp);
	fclose(fp);

	strcpy(pathlinks, pre_load3("/links.htm"));
	fp = fopen(pathlinks,"r");
	fread (htmlinks, sizeof(char), 4472, fp);
	fclose(fp);

	strcpy(pathindex, pre_load3("/index.html"));
	fp = fopen(pathindex,"r");
	fread (htmindex, sizeof(char), 4132, fp);
	fclose(fp);

	strcpy(pathishop, pre_load3("/ishop.htm"));
	fp = fopen(pathishop,"r");
	fread (htmishop, sizeof(char), 6694, fp);
	fclose(fp);

	strcpy(pathilinks, pre_load3("/ilinks.htm"));
	fp = fopen(pathilinks,"r");
	fread (htmilinks, sizeof(char), 3955, fp);
	fclose(fp);

	strcpy(pathicontact, pre_load3("/icontact.htm"));
	fp = fopen(pathicontact,"r");
	fread (htmicontact, sizeof(char), 2476, fp);
	fclose(fp);

	strcpy(pathcontact, pre_load3("/contact.htm"));
	fp = fopen(pathcontact,"r");
	fread (htmcontact, sizeof(char), 4474, fp);
	fclose(fp);

	strcpy(pathhome, pre_load3("/home.htm"));
	fp = fopen(pathhome,"r");
	fread (htmhome, sizeof(char), 4470, fp);
	fclose(fp);

	strcpy(pathbullet3, pre_load3("/bullet3.jpg"));
	fp = fopen(pathbullet3,"r");
	fread (jpgbullet3, sizeof(char), 766, fp);
	fclose(fp);

	strcpy(pathbullet2, pre_load3("/bullet2.jpg"));
	fp = fopen(pathbullet2,"r");
	fread (jpgbullet2, sizeof(char), 792, fp);
	fclose(fp);

	strcpy(pathbullet1, pre_load3("/bullet1.jpg"));
	fp = fopen(pathbullet1,"r");
	fread (jpgbullet1, sizeof(char), 802, fp);
	fclose(fp);

	strcpy(pathbanner3, pre_load3("/banner3.jpg"));
	fp = fopen(pathbanner3,"r");
	fread (jpgbanner3, sizeof(char), 18167, fp);
	fclose(fp);

	strcpy(pathbanner2, pre_load3("/banner2.jpg"));
	fp = fopen(pathbanner2,"r");
	fread (jpgbanner2, sizeof(char), 8612, fp);
	fclose(fp);

	strcpy(pathbanner1, pre_load3("/banner1.jpg"));
	fp = fopen(pathbanner1,"r");
	fread (jpgbanner1, sizeof(char), 12563, fp);
	fclose(fp);

	strcpy(pathbac_04, pre_load3("/images/bac_04.jpg"));
	fp = fopen(pathbac_04,"r");
	fread (jpgbac_04, sizeof(char), 64043, fp);
	fclose(fp);

	strcpy(path0501, pre_load3("/images/05_01.gif"));
	fp = fopen(path0501,"r");
	fread (gif0501, sizeof(char), 16656, fp);
	fclose(fp);

	strcpy(path0502, pre_load3("/images/05_02.gif"));
	fp = fopen(path0502,"r");
	fread (gif0502, sizeof(char), 150943, fp);
	fclose(fp);

	strcpy(path0503, pre_load3("/images/05_03.gif"));
	fp = fopen(path0503,"r");
	fread (gif0503, sizeof(char), 24764, fp);
	fclose(fp);

	strcpy(path0504, pre_load3("/images/05_04.gif"));
	fp = fopen(path0504,"r");
	fread (gif0504, sizeof(char), 64042, fp);
	fclose(fp);

	strcpy(pathover0504, pre_load3("/images/05_04-over.gif"));
	fp = fopen(pathover0504,"r");
	fread (overgif0504, sizeof(char), 8145, fp);
	fclose(fp);

	strcpy(pathover05040507, pre_load3("/images/05_04-05_07_over.gif"));
	fp = fopen(pathover05040507,"r");
	fread (overgif05040507, sizeof(char), 8113, fp);
	fclose(fp);

	strcpy(path0505, pre_load3("/images/05_05.gif"));
	fp = fopen(path0505,"r");
	fread (gif0505, sizeof(char), 13968, fp);
	fclose(fp);

	strcpy(path0506, pre_load3("/images/05_06.gif"));
	fp = fopen(path0506,"r");
	fread (gif0506, sizeof(char), 8618, fp);
	fclose(fp);

	strcpy(pathover0506, pre_load3("/images/05_06-over.gif"));
	fp = fopen(pathover0506,"r");
	fread (overgif0506, sizeof(char), 8494, fp);
	fclose(fp);

	strcpy(path0507, pre_load3("/images/05_07.gif"));
	fp = fopen(path0507,"r");
	fread (gif0507, sizeof(char), 8514, fp);
	fclose(fp);

	strcpy(pathover0507, pre_load3("/images/05_07-over.gif"));
	fp = fopen(pathover0507,"r");
	fread (overgif0507, sizeof(char), 8453, fp);
	fclose(fp);

	strcpy(path0508, pre_load3("/images/05_08.gif"));
	fp = fopen(path0508,"r");
	fread (gif0508, sizeof(char), 9510, fp);
	fclose(fp);

	strcpy(pathover0508, pre_load3("/images/05_08-over.gif"));
	fp = fopen(pathover0508,"r");
	fread (overgif0508, sizeof(char), 9325, fp);
	fclose(fp);

	strcpy(path0509, pre_load3("/images/05_09.gif"));
	fp = fopen(path0509,"r");
	fread (gif0509, sizeof(char), 119545, fp);
	fclose(fp);

	strcpy(path0510, pre_load3("/images/05_10.gif"));
	fp = fopen(path0510,"r");
	fread (gif0510, sizeof(char), 1391, fp);
	fclose(fp);
}

char* pre_load3(char *filename)
{
	char *p = (char *)malloc(512);
	char path[512] = {};
	
	strcpy(path,basicpath);
	strcat(path,filename);
	strcpy(p,path);

	return p;
}
