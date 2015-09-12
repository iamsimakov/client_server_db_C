#include <iostream>
#include <cstdio>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    
    if (argc < 5) {
       cout <<"Usage: " << argv[0] << " {hostname} {port} {get|set} {key}\n";
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        cout << stderr << " ERROR, no such host\n";
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	
	n = write(sockfd,argv[3],strlen(argv[3]));
	if (n < 0) error("ERROR writing to socket");
        n = write(sockfd,argv[4],strlen(argv[4]));
	if (n < 0) error("ERROR writing to socket");
	
	if (strcmp(argv[3],"set")==0 ) {		
		cout << "Please enter the value: ";
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		n = write(sockfd,buffer,strlen(buffer));
        	if (n < 0) error("ERROR writing to socket");
	}
	
	bzero(buffer,256);
	n = read(sockfd,buffer,256);
	if (n < 0) error("ERROR reading from socket");
	cout << buffer << "\n";

    close(sockfd);
    return 0;
}
