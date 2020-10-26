#define MSGSIZE 512
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
char *reply= 
"HTTP/1.1 200 OK\n"
"Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
"Server: Apache/2.2.3\n"
"Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
"ETag: \"56d-9989200-1132c580\"\n"
"Content-Type: text/html\n"
"Content-Length: 15\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
"sdfkjsdnbfkjbsf";
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[MSGSIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int flag = 1;
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
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     while (flag){
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,MSGSIZE);
     n = read(newsockfd,buffer,MSGSIZE);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     if (strstr(buffer, "STOP") != NULL)
	     flag = 0;
     bzero(buffer,MSGSIZE);
     if (flag)
	     n = write(newsockfd,reply,strlen(reply));
     else
	     n = write(newsockfd,"STOPPING",8);
     if (n < 0) error("ERROR writing to socket");
     }
     return 0; 
}
