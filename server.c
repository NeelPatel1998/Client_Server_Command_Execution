/*
	Server Code for executing commands sent by client on server and return the output
	
	Team Members:
	Neel Patel (110035184)
	Sayee Shruthi Anantharaman SathyaPrakash (110052774)
	
	Date: 18 August 2021
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

// Declaration of function used
void serviceClient(int);

int main(int argc, char *argv[]){  
	int sock, client, portNo, ret;
	struct sockaddr_in serverAddr;
  	
  	// 1 Argument is needed : Port Number
 	if(argc != 2){
    	fprintf(stderr, "Usage: %s <Port Number>\n", argv[0]);
   	 	exit(0);
  	}
  	// Create Socket
  	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	fprintf(stderr, "Could  not create socket\n");
    	exit(0);
  	}
  	
  	// Creating socket
  	sock = socket(AF_INET, SOCK_STREAM, 0);
  	serverAddr.sin_family = AF_INET;
  	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  	sscanf(argv[1], "%d", &portNo);
  	serverAddr.sin_port = htons((uint16_t)portNo);
  
  	// Binding socket to the port
  	bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  	listen(sock, 5);

	printf("Waiting for a client....\n");
    	
    // Server infinitely accepts new client
  	while(1){
  		
  		// On getting a client connection
    	client = accept(sock, NULL, NULL);
    	printf("CLIENT CONNECTED\n");
    	
    	// for each client serviceClient function is called
    	if(!fork())
    		serviceClient(client);
		
		// Close connection after client disconnects
    	close(client);
    	waitpid(0, &ret, WNOHANG);
  	}
}

void serviceClient(int sock){
	char cmd[255];
    int n, pid;
    write(sock, "CONNECTED", strlen("CONNECTED")+1);

	// Read and execute command from client
	while(1){
		if(n=read(sock, cmd, 255)){
			cmd[n]='\0';
			fprintf(stderr,"%s", cmd);
			
			// Close connection if quit command is recieved
			if(!strcmp(cmd, "quit\n")){
				exit(0);
			}
			
			// Redirecting the output of system() to client
			dup2(sock, STDOUT_FILENO);
			
			// Executing the command from client
			system(cmd);
		}
	}
}
