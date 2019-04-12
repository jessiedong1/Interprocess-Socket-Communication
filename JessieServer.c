//Zeqing Dong & Lucas Clementson
//Inter-Process Socket Communication under Linux OS
//Source code is from http://www.linuxhowtos.org/C_C++/socket.htm

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define TERMINATED 1

/*This function is called when a system call fails. 
It displays a message about the error on stderr 
and then aborts the program.*/
void error(const char *msg){
	perror(msg);
	exit(-1);
}

/*This function is sending acknowledge from server to client*/
void sendAck(int newsockfd, int clientId, int code){
	int n;
	const char *message = "I got your message !";
    // Check to see if connection was terminated or not 
	if (code == 0){
		//Connection was not terminated
		//Send acknowledgment message to the client
		n = write(newsockfd, message, strlen(message));
	} else {
		//Connection was terminated
		message = "Connection Terminated!";

		//Send acknowledgment message to the client
		n = write(newsockfd, message, strlen(message));
		printf("Client %d -- %s\n", clientId, message);
		close(newsockfd);
	}
	
	if (n < 0)
		error("ERROR writing to socket.");
}


/*This funcation is printing the message send by clients*/
int printMessage(int newsockfd, int clientId){
	char buffer[256];
	//Zero out the buffer
	bzero(buffer, 256);
	int n = read(newsockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket.");

	//Check if the client wanted to terminate its connection
	if(strcmp(buffer, "\\Terminate\n") == 0)
		return TERMINATED;

	//Make sure the buffer holds valid data
	if(n > 0 && buffer[0] != 0);
		printf("Here is the message from client %d: %s", clientId, buffer);
	return 0;
}

int main(int argc, char *argv[]){

	//Declare and initialize client ID 
	int clientId = 0;

	//Get process ID
	int pid = getpid();
	//sockfd and newsockfd are file descriptors.

	//portno stores the port number on which the server accepts connections.

	//clilen stores the size of the address of the client. This is needed for 		the accept system call.

	//n is the return value for the read() and write() calls.

	int sockfd, newsockfd, portno, clilen, n;

	//A sockaddr_in is a structure containing an internet address. 
	struct sockaddr_in servAddr, cliAddr;

	/*The user needs to pass in the port number on 
	which the server will accept connections as an argument.*/
	if (argc < 2){
		fprintf(stderr, "ERROR, no port provided.");
		exit(-1);
	}
	// terminated child process when the parent is forced to terminate
	signal(SIGCHLD, SIG_IGN);
	
	
	
	//Create a new socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Check if socket call fails or not 
	if (sockfd < 0)
		error("ERROR opening socket.");
	
	//sets all values in a buffer to zero.
	bzero((char *) &servAddr, sizeof(servAddr));
	
	//The port number on which the server will listen
	portno = atoi(argv[1]);

	//Set short sin_family, which contains a code for the address family
	//Into AF_INET.
	servAddr.sin_family = AF_INET; 

	//Set unsigned short sin_port, which contain the port number, to a port 	//number in host byte order to network order
	servAddr.sin_port = htons(portno);

	//Set unsigned long s_addr. to be INADDR_ANY
	servAddr.sin_addr.s_addr = INADDR_ANY;
	
	/*The bind() system call binds a socket to an address, 
	in this case the address of the current host and port 
	number on which the server will run.*/
	if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		error("ERROR on binding.");
	/*The listen system call allows the process to listen on the socket for 	connections.Here set the maximum size to 5*/
	listen(sockfd, 5);

	clilen = sizeof(cliAddr);
	
	while(1){
		//Check for the status of the new connection
		if (newsockfd < 0)
		     error("ERROR on accept");
		//Check for the status of the new process
		if (pid < 0)
		     error("ERROR on fork");
		if (pid == 0){
			//The new process was created successfully
			//Handle request here
		    if (printMessage(newsockfd, clientId)){
				close(sockfd);
				sendAck(newsockfd, clientId, TERMINATED);
				exit(0);
			}
			sendAck(newsockfd, clientId, 0);
		} else {
			//Parent process
			++clientId;
			newsockfd = accept(sockfd, (struct sockaddr *) &cliAddr, &clilen);
			pid = fork();
		}
	}
    //Close server 
	close(newsockfd);
    close(sockfd);
	return 0;
}

