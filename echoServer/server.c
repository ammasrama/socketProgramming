/* Aum Amriteswaryai Namaha
 * Aum Shri Ganeshaya Namaha
 *
 * File: Server.c
 * Owner: Sriram Karunagaran
 * Description; Networking Server program to echo client data
 */
 
#include "global.h"

void str_echo(int);

int main(int argc, char *argv[])
{
    int       listenFileDescriptor; 
    int       connectFileDesriptor;
    int       returnValue;
    int       errno;

    pid_t     childPid;
     
    struct    sockaddr_in serverAddress;
    struct    sockaddr_in clientAddress;
    socklen_t clientLen;

    
    /* STEP1 - call the socket function */
    /* family - IPv4 socket (AF_INET),
     * type   - SOCK_STREAM, stream socket
     * protocol - IPPROTO_TCP - TCP protocol
     */
    listenFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenFileDescriptor < 0)
    {
        perror("Socket error");
        exit(0);
    }
    
    /* STEP2 - call the bind function */
    /* clear the contents of serverAddress */
    bzero(&serverAddress, sizeof(serverAddress));

    /* set the value for server ip_address and port
     * let the OS set the IPaddress using inaddr_any
     * convert the address and port to network format
     */
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port        = htons(SERV_PORT);

    /* STEP3 - call bind function */
    /* This creates a passive connection */
    returnValue = bind(listenFileDescriptor, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
    if(returnValue == -1) 
    {
        perror("Bind error");
        exit(0);
    }

    /* STEP4 - call listen function with parameter*/
    /* Wait for connections from client*/
    returnValue = listen(listenFileDescriptor, LISTENQ_SIZE);
    if(returnValue == -1) 
    {
        perror("Listen error");
        exit(0);
    }

    for (;;) 
    {
        clientLen = sizeof(clientAddress);
        connectFileDesriptor = accept(listenFileDescriptor, (struct sockaddr *) &clientAddress, &clientLen);

        childPid = fork();

        /* if child process */
        if (childPid == 0)
        {
            close(listenFileDescriptor);
            str_echo(connectFileDesriptor);
            exit(0);
        }

        /* closing the connect descriptor by both the child and parent */
        close(connectFileDesriptor);
    }

    return 0;
}


void str_echo(int sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];
    int     errno;
    
again:
    memset(buf, 0, MAXLINE);
    while ((n= read(sockfd, buf, MAXLINE)) > 0)
    {
	    printf("%s\n", buf);
        write(sockfd, buf, n);
        memset(buf, 0, MAXLINE);
    }

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        perror("Read Error");
}
