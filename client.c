/*
	Client Code for executing commands sent by client on server and return the output
	
	Team Members:
	Neel Patel (110035184)
	Sayee Shruthi Anantharaman SathyaPrakash (110052774)
	
	Date: 18 August 2021
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>


int main(int argc, char *argv[]){
  	char cmd[255];
  	int server, portNo, pid, n;
  	struct sockaddr_in serverAddr;
  
  	// 2 Arguments are needed : IP Address & Port Number
 	if(argc != 3){
    	fprintf(stderr, "Usage: %s <IP Address> <Port Number>\n", argv[0]);
    	exit(0);
  	}
	
	// Create Socket
  	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	fprintf(stderr, "Could not create socket\n");
    	exit(0);
  	}
  	
	// Connecting to server using socket
  	serverAddr.sin_family = AF_INET;
  	sscanf(argv[2], "%d", &portNo);
	serverAddr.sin_port = htons((uint16_t)portNo);
	inet_pton(AF_INET, argv[1], &serverAddr.sin_addr);

	if(connect(server, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0){
		perror("ERROR: connect() failed! Try different port \n");
		exit(0);
	}
	
	// Read first connection message from server
  	read(server, cmd, 255);
  	fprintf(stderr, "Received: %s\n", cmd);
	fprintf(stderr, "Enter your commands to be executed on server: \n");
	
	// Fork for creating 2 processes
  	pid=fork();
  	
  	// This process continously reads from server and displays it to user
  	if(pid){
		while(1){
			if(n=read(server, cmd, 255)){
		    	cmd[n]='\0';
		    	printf("%s\n", cmd);
		   	}
		}                         
	}
	
	// This process continously reads from user and writes it to server
 	if(!pid){
		while(1){
		 	 // Read command from user(0 is STDIN) 
			 if(n=read(0, cmd, 255)){
				 cmd[n]='\0';
				 fprintf(stderr, "%s", cmd);
				 
				 // Write command to server
				 write(server, cmd, strlen(cmd)+1);
				 
				 if(!strcmp(cmd, "quit\n")){
				 	// Kill other process which is reading and exit
				 	kill(pid, SIGKILL);
				 	exit(0);
				 }
		  	}
		}
	}
}


