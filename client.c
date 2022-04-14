#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define LIMIT 1024
#define SA struct sockaddr

int open_socket(int PORT_NUMBER){
        int sockfd;
        int connfd;


    struct sockaddr_in servaddr, cli;
   

   //creat a TCP socket , using ipv4 address

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

   // socket function returns -1 if the socket failed
   // to be created

    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }


    //socket created

    else
        printf("Socket successfully created..\n");
        // assign servaddr with zero values
        bzero(&servaddr, sizeof(servaddr));
   


    // assign IP
    // assign PORT_NUMBER
    // using "127.0.0.1" local host

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT_NUMBER);
   


    // connect the  two sockets :
    // client socket and server socket



    //check if the connection succefully created
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }

    
    else
        printf("connected to the server..\n");
   


    // dup2 is used to communcite  between the two:
    // client and server
 
    dup2(sockfd,1);


    return sockfd;
}


//this function is used to return for the stdout(standardoutput)
// after getting a specific command from  clien (LOCAL)

int local_socket(int sockfd,int std_output){

    close(sockfd);

    dup2(std_output,1);
    return 0;
}