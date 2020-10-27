#define MSGSIZE 1024
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<signal.h>
struct htmlresponse {
	int code;
	char date[29];
	char server[29];
	char lastModified[29];
	int contentLength;
	char contentType[29];
	char connection[10];

};
char * generateresponse(struct htmlresponse meta,char content[], char * response){
	char codename[10];
	// making date entry
	if (meta.code == 200)
		strcpy(codename, "OK");
	else
		strcpy(codename, "yeet");
	snprintf(response, MSGSIZE, "HTTP/1.1 %d %s\n Date: %s\n Server: %s\n Last-Modified: %s\n Content-Type: %s\n Content-Length: %d\n Connection: %s\n%s",meta.code, codename, meta.date, meta.server,meta.lastModified, meta.contentType, meta.contentLength, meta.connection);
	return (char *)response;
}
void error(char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
signal(SIGPIPE, SIG_IGN);
char *gamercontent =
"<b>YOU HAVE REACHED GAMER PAGE</b>";
char gamerreply[MSGSIZE]= 
"HTTP/1.1 200 OK\n"
"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
"Server: Apache/2.2.3\n"
"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
"ETag: \"56d-9989200-1132c580\"\n"
"Content-Type: text/html\n"
"Content-Length: 50\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"; 
strcat(gamerreply,gamercontent);
char *content =
"<p>hit the quan</p>"
"<p><a href=\"/gamer\">hit the quaran</a></p>";
char reply[MSGSIZE]= 
"HTTP/1.1 200 OK\n"
"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
"Server: Apache/2.2.3\n"
"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
"ETag: \"56d-9989200-1132c580\"\n"
"Content-Type: text/html\n"
"Content-Length: 50\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"; 
strcat(reply,content);
     int sockfd, newsockfd, portno, clilen;
     char buffer[MSGSIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int flag = 1;
     int gamer = 0;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (flag){
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,MSGSIZE);
     n = read(newsockfd,buffer,MSGSIZE);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the request: %s\n",buffer);
     //if (strstr(buffer, "STOP") != NULL)
//	     flag = 0;
     if (strstr(buffer,"/gamer") != NULL)
	     gamer = 1;
     else
	     gamer = 0;
     printf("gamer is %d", gamer);
     bzero(buffer,MSGSIZE);
     if (flag){
//	     printf("%d\n%s", strlen(reply), reply);
	     if (gamer)     
	     	n = write(newsockfd,gamerreply,strlen(gamerreply));
	     else
	     n = write(newsockfd,reply,strlen(reply));
     }
	else
	     n = write(newsockfd,"STOPPING",8);
     if (n < 0) error("ERROR writing to socket");
     }
     return 0; 
}
