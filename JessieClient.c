//Zeqing Dong & Lucas Clementson
//Inter-Process Socket Communication under Linux OS: Client
//Source code is from http://www.linuxhowtos.org/C_C++/socket.htm


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


/*This function is called when a system call fails. 
It displays a message about the error on stderr 
and then aborts the program.*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //sockfd and newsockfd are file descriptors.

    //portno stores the port number on which the server accepts connections.

   //n is the return value for the read() and write() calls.
    int sockfd, portno, n;
   
    //The variable serv_addr will contain the address of the server to which we want to connect.
    struct sockaddr_in serv_addr;
    //The variable server is a pointer to a structure of type hostent.
    struct hostent *server;
    
    char buffer[256];
    // Check if the input has three arguments
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    //The port number on which the server will listen
    portno = atoi(argv[2]);

    //Create a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //Check if socket call fails or not 
    if (sockfd < 0) 
        error("ERROR opening socket");
    //Get the host name from second argument
    server = gethostbyname(argv[1]);
    //Check if host name is provided
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    //sets all values in a buffer to zero.
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //Set short sin_family, which contains a code for the address family
	//Into AF_INET.
    serv_addr.sin_family = AF_INET;
    
    //Sets the fields in serv_addr
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    //Check if the connection
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
   
    //Sending multiple messages to server
    while(1){
            printf("Please enter the message(Enter t to terminate): ");
            //Zero out the buffer
            bzero(buffer,256);
            // read the message from stdin
            fgets(buffer,255,stdin);
	    //Close the connection
           if(buffer[0] == 't')
            {
                close(sockfd);
                return 0;
            }
            //writes the message to the socket
            n = write(sockfd,buffer,strlen(buffer));
	    // Check if the writing has error
            if (n < 0) 
                error("ERROR writing to socket");
            bzero(buffer,256);
	    //read the message from the socket
            n = read(sockfd,buffer,255);
	    // Check if the reading has error
            if (n < 0) 
                error("ERROR reading from socket");
            printf("%s\n",buffer);
        
            
           }
      

   
   
}
