#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT_NUMBER 55555
#define LIMIT 1024

#define SA struct sockaddr
   





void func(int connfd)
{
    char buf[LIMIT];

    bzero(buf, LIMIT);


    while(read(connfd, buf, sizeof(buf)) && strcmp(buf, "LOCAL\n")!=0) {

        printf("%s\n", buf);

        bzero(buf, LIMIT);

    }
  
}
   


int main()
{
    int sockfd;
    int connfd;
    int length;


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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT_NUMBER);
   



    // Bind the socket to local host  and port...

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   




    // listen

    if ((listen(sockfd, 1)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }


    else
        printf("Server listening..\n");
        length = sizeof(cli);


    while (1)
{
   
    // Accept and open connection socket
    connfd = accept(sockfd, (SA*)&cli, &length);

    if (connfd < 0) {
        printf("failed to accept\n");
        exit(0);
    }

    
    else
        printf("server accepted succefully\n");
   

    // used to communcite  between the two:
    // client and server
    func(connfd);
   
    //to close the connection socket
    close(connfd);

}


}