#define MSGSIZE 2048
#include<regex.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<signal.h>
struct httpresponse {
	int code;
	char date[29];
	char server[29];
	char lastModified[31];
	int contentLength;
	char contentType[29];
	char connection[10];

};
struct httprequest {
	char type[10];
	char path[10];
	char host[29];
	char userAgent[200];
	char acceptEncoding[40];
	char acceptLanguage[40];
};
char * getpath(char * request){
	const char *before = "GET ";
	const char *after = " HTTP";
	const char *lowbound = strstr(request, before);
	if (lowbound != NULL){
		const size_t pl1 = strlen(before);
		printf("aaa \n%s\n", lowbound);
		const char *highbound = strstr(lowbound + pl1, after);
		if (highbound != NULL){
			const size_t mlen = highbound - (lowbound + pl1);
			char *ret = malloc(mlen + 1);
			if (ret != NULL) {
				memcpy(ret, lowbound + pl1, mlen);
				ret[mlen] = "\0";
				return ret;
			}
			else
				printf("malloc fail");
		}
	else
	printf("highbound fail");
	}
	else
	printf("lownound fail");
}
struct httprequest parserequest(char request[]){
}
char * generateresponse(struct httpresponse meta,char *content, char * response){
	char codename[10];
	// making date entry
	if (meta.code == 200)
		strcpy(codename, "OK");
	else
		strcpy(codename, "yeet");
	bzero(response, MSGSIZE);
	int n = snprintf(response, sizeof(char[MSGSIZE]), "HTTP/1.1 %d %s\n Date: %s\n Server: %s\n Last-Modified: %s\n Content-Type: %s\n Content-Length: %d\n Connection: %s\n\n%s",meta.code, codename, meta.date, meta.server,meta.lastModified, meta.contentType, meta.contentLength, meta.connection, content);
	printf("\n%d\n", n);
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
"<b>YOU HAVE REACHED GAMER PAGE</b>"
"<div class=\"tenor-gif-embed\" data-postid=\"10377631\" data-share-method=\"host\" data-width=\"100%\" data-aspect-ratio=\"1.375\"><a href=\"https://tenor.com/view/nyan-cat-gif-10377631\">Nyan Cat GIF</a> from <a href=\"https://tenor.com/search/nyancat-gifs\">Nyancat GIFs</a></div><script type=\"text/javascript\" async src=\"https://tenor.com/embed.js\"></script> a";
char *content =
"<p>hit the quan</p>"
"<p><a href=\"/gamer\">hit the quaran</a></p>";
struct httpresponse game = {200, "Thu, 19 Feb 2009 12:27:04 GMT","Apache/2.2.3","Wed, 18 Jun 2003 16:05:58 GMT", 50, "text/html","close"};
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
     while (1){
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,MSGSIZE);
     n = read(newsockfd,buffer,MSGSIZE);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the request: %s\n",buffer);
	char * path = getpath((char *)buffer);
    printf("Here is the requested path: %s\n", path);	
     if (strstr(buffer, "/stop") != NULL){
		printf("exiting!");	  
		close(newsockfd);
		break;
     }
     bzero(buffer,MSGSIZE);
	     n = write(newsockfd,"STOPPING",8);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     }
     return 0; 
}
