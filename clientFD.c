/*
 * Example of client using TCP protocol.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>

// You may/may not use pthread for the client code. The client is communicating with
// the server most of the time until he recieves a "GET <file>" request from another client.
// You can be creative here and design a code similar to the server to handle multiple connections.

FILE * getFiles(){
	return popen("ls sharedFiles/", "r");
}

int main(int argc, char *argv[]) {

	char s[1000];
	char * pch;
	char filePath[1000] = "";
	struct stat st;

	if(argc != 4) {
		printf("Usage: ./clientFD <HOST> <PORT_NUMBER> <COMMA ',' DELINEATED FILE LIST>\n");
		exit(1);
	}

	struct addrinfo hints, *res;
	int sockfd;

	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(argv[1], argv[2], &hints, &res);

	// make a socket:

	if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
		perror("socket()");
		exit(1);
	}


	// connect it to the address and port we passed in to getaddrinfo():

	if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		perror("connect()");
		exit(1);
	}
	
	pch = strtok(argv[3], ",");

	while(pch != NULL){

		bzero(filePath, sizeof(filePath));

		strcat(filePath, "sharedFiles/");
		strcat(filePath, pch);
		
		// Print file names
		printf("File: %s\n", filePath);

		// Print file size
		stat(filePath, &st);
		printf("File size: %d bytes\n", st.st_size);
		

		pch = strtok(NULL, ",");
	}

	/*if(send(sockfd, fileList, sizeof(fileList), NULL) < 0) {
		perror("send()");
		exit(1);
	}*/

	while(1) {

		gets(s);
		if(send(sockfd, s, sizeof(s), NULL) < 0) {
			perror("send()");
		}
		
	}

	return 0;
}
