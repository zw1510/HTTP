#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/sendfile.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/types.h>
#include<string.h>

#define SIZE 1024
int gsock =0;
static void usage(const char* proc)
{
		printf("Usage: %s [ip] [port]\n",proc);
}

void clear_head(int sock)
{
	char buf[SIZE];
	int len=SIZE;
	int ret = -1;
	do{
			ret = get_line(sock,buf,len);
	}while((ret > 0) && strcmp(buf,"\n") != 0);
}
static int startup(const char* _ip,const int _port)
{
		printf("bbbbbbbbbbbbbb");
		int sock = socket(AF_INET,SOCK_STREAM,0);
		if(sock < 0)
		{
				perror("socket");
				exit(2);
		}
		
		int opt = 1;
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

		struct sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = inet_addr(_ip);
		local.sin_port = htons(_port);

		if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
		{
				perror("bind");
				exit(3);
		}

		if(listen(sock,5) < 0)
		{
				perror("listen");
				exit(4);
		}
		return sock;
}
int get_line(int sock,char buf[],int len)
{
		if(!buf || len < 0)
		{
			return -1;
		}
		int ret = 0;
		int i = 0;
		char ch = '\0';
		int n = 0;
		while(i < len && ch != '\n')
		{
				ret = recv(sock,&ch,1,0);
				if(ret > 0)
				{
					if(ch == '\r')
					{
							n = recv(sock,&ch,1,MSG_PEEK);//查看当前数据。数据将被复制到缓冲区中，但并不从输入队列中删除。
							if(n > 0 )
							{
								if( ch == '\n')//windows
								{
										recv(sock,&ch,1,0);
								}
								else
								{
										ch = '\n';
								}
							}
					}
					buf[i++] = ch;
				}
				else
				{
						ch = '\n';
				}
		}
		buf[i] = '\0';

		return i;
}
//void not_found(int sock)
//{
//		char buf[1024];
//		memset(buf,'\0',sizeof(buf)/sizeof(buf[0]));
//		sprintf(buf,"/HTTP/1. 404 NOT FOUND");
//		send(sock,buf,strlen(buf),0);
//		sprintf(buf,"Content type: type/html\r\n");
//		send(sock,buf,strlen(buf),0);
//		send(sock,"\r\n",strlen("\r\n"),0);
//
//		sprintf("<html><head><h2>FILE NOT FOUND !!!</h2></head");
//}
void echo_errno(int sock,int number)
{
		switch(number){
				case 404:
					//	now_found();
						break;
				case 400:
						break;
				case 401:
						break;
				case 403:
						break;
				case 500:
						break;
				default:
						break;
		}
}

static void echo_www(int sock,const char* path,ssize_t size)
{
	int fd = open(path,O_RDONLY);
	if(fd < 0)
	{
			echo_errno(sock,1);//
			return ;
	}
	printf("get a new client: %d -> %s\n",sock,path);

	char buf[SIZE];
	sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
	send(sock,buf,strlen(buf),0);

	if(sendfile(sock,fd,NULL,size) < 0)//sendfile  提高效率
	{
			echo_errno(sock,1);
			return ;
	}

	close(fd);
}

static void execut_cgi(int sock,const char* path,const char* method,const char* query_string)
{
		int content_len = -1;
		char buf[SIZE];
		memset(buf,'\0',sizeof(buf));
		int cgi_input[2];
		int cgi_output[2];
		
		printf("method: %s\n",method);


		if(strcasecmp(method,"GET") == 0)
		{
				clear_head(sock);
		}else  //POST
		{
				printf("aaaaaaaaaaaaaaaa\n");
				int ret = 0;
				do{
						ret = get_line(sock,buf,sizeof(buf));
						if(strncasecmp(buf,"Content-Length: ",16) == 0)
						{
								content_len = atoi(&buf[16]);				
						}
				}while((ret > 0) && strcmp(buf,"\n") != 0);
				printf("contlen:%d\n",content_len);
				if(content_len == -1)
				{
						echo_errno(sock,1);
						return ;
				}
		}
		if(pipe(cgi_output) < 0)
		{
			echo_errno(sock,1);
			return ;
		}
		if(pipe(cgi_input) < 0)
		{
			echo_errno(sock,1);
			return ;
		}

				
		sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
		send(sock,buf,strlen(buf),0);
		pid_t id = fork();
		// 处理客户端请求
		if(id == 0)//child
		{
				close(cgi_input[1]);
				close(cgi_output[0]);
		
				dup2(cgi_input[0],0);
				dup2(cgi_output[1],1);
				
				sprintf(buf,"REQUEST_METHOD=%s",method);
				putenv(buf);

				if(strcasecmp(method,"GET") == 0)
				{
						sprintf(buf,"QUERY_STRING=%s",query_string);
						putenv(buf);
				}else
				{
						sprintf(buf,"CONTENT_LENGTH=%d",content_len);
						putenv(buf);
				}
				execl(path,path,NULL);
				exit(1);
		}else  //father
		{
				close(cgi_input[0]);
				close(cgi_output[1]);

				int i=0;
				char c = '\0';
				if(strcasecmp(method,"POST")==0)
				{
						printf("cccccccccccc\n");
						for(;i < content_len;++i)
						{
								recv(sock,&c,1,0);
								write(cgi_input[1],&c,1);
						}
				}
				printf("\n");

				while(read(cgi_output[0],&c,1) > 0)
				{
						send(sock,&c,1,0);
				}

				waitpid(id,NULL,0);
				close(cgi_input[1]);
				close(cgi_output[0]);

		}
}
static void * accept_request(void* arg)
{
	int sock = (int)arg;
	printf("%d\n",sock);
	char buf[SIZE];
	int ret = 0;
#ifdef _DEBUG_
	do
	{
			ret = get_line(sock,buf,size);
			printf("%s",buf);
			fflush(stdout);
	}while(ret > 0 && strcmp(buf,'\n'));
#endif
	char method[SIZE / 10];
	char url[SIZE];

	char* query_string = NULL;
	int cgi = 0;
	char path[SIZE];
	memset(method,'\0',sizeof(method));
	memset(url,'\0',sizeof(url));

	ret = get_line(sock,buf,sizeof(buf));
	if(ret < 0)
	{
			echo_errno(sock,1);
			return (void*)1;
	}
	printf("%s\n",buf);
	//1 get method
	//GET //xx//yy HTTP/1.1
	int i = 0;
	int j =0;
	while(i< sizeof(method) - 1 && !isspace(buf[j])  && j < sizeof(buf) )
	{
			method[i]=buf[j];
			++i;
			++j;
	}

	method[i] = '\0';

	//2 check method
	if(strcasecmp(method,"GET")!=0 && strcasecmp(method,"POST") != 0)
	{
			echo_errno(sock,1);
			return (void*)2;
	}

	//3 get url first step space
	while(isspace(buf[j]))
	{
			++j;
	}

	i=0;
	while(!isspace(buf[j])&& (i < sizeof(url)-1) && j < sizeof(buf))
	{
		url[i] = buf [j];
		++i;
		++j;

	}

	if(strcasecmp(method,"POST") == 0)
	{
			cgi = 1;
	}
	printf("method: %s,url_path :%s\n ",method,url);

	if(strcasecmp(method,"GET") == 0)
	{
			query_string = url;
			
	}

	char *start = url;
	while( *start != '\0')
	{
			if( *start == '?')
			{
					cgi = 1;
					*start = '\0';
					query_string = start+1;
					break;
			}

			++start;

	}
	sprintf(path,"htdoc%s",url);
	if(path[strlen(path)-1] == '/')
	{
			strcat(path,"index.html");
	}

	//
	printf("path:%s\n",path);
	printf("cgi:%d\n",cgi);
	//method,query_string,cgi,path
	struct stat st;
	if(stat(path,&st) < 0)//default -> htdoc/index.html
	{
			printf("stat error\n");
			echo_errno(sock,1);
			return (void*)-3;
			
	}
	else
	{
			if(S_ISDIR(st.st_mode))
			{
					strcpy(path,"htdoc/index.html");
			}
			if(( st.st_mode & S_IXGRP)||\
					( st.st_mode & S_IXGRP)||\
					st.st_mode & S_IXOTH)
			{
					cgi = 1;
			}
			else
			{}

			//path cgi
			if(cgi)
			{
					printf("cgi mode\n");
					execut_cgi(sock,path,method,query_string);
			}
			else// 请求首页内容
			{
					clear_head(sock);
					echo_www(sock,path,st.st_size);
			}
	}

	close(sock);   //no face link
	return (void*) 0;
}
int main(int argc,char* argv[])
{
		if(argc != 3)
		{
				usage(argv[0]);
				exit(1);
		}

		int listen_sock = startup(argv[1],atoi(argv[2]));
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int done=0;
		while(!done)
		{
				int new_fd = accept(listen_sock,(struct sockaddr*)&peer,&len);
				if(new_fd < 0)
				{
						sleep(2);
						perror("accpet");
						continue;
				}
				printf("get a new client\n");
				pthread_t id;
				pthread_create(&id,NULL,accept_request,(void*) new_fd);
				pthread_detach(id);
		}
		return 0;
	
}
