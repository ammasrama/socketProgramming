/* Aum Amriteswaryai Namaha
 * Aum Shri Ganeshaya Namaha
 *
 * File: Server.c
 * Owner: Sriram Karunagaran
 * Description; Networking Server program to echo client data
 */
 
#include "global.h"

typedef struct socketStruct
{
        int sourceSocket;
        int destinationSocket;
} socketStruct_t;

void *clientInterconnect(void *);

int main()
{
    int       listenFileDescriptor; 
    int       connect1FileDescriptor;
    int       connect2FileDescriptor;
    int       returnValue;
    int       errno;

    struct    sockaddr_in serverAddress;
    struct    sockaddr_in clientAddress;
    socklen_t clientLen;

    pthread_t tid1, tid2; // two threads
    pthread_attr_t attr; // thread attributes

    socketStruct_t client1Attributes, client2Attributes;

    
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

    clientLen = sizeof(clientAddress);
    connect1FileDescriptor = accept(listenFileDescriptor, (struct sockaddr *) &clientAddress, &clientLen);

    clientLen = sizeof(clientAddress);
    connect2FileDescriptor = accept(listenFileDescriptor, (struct sockaddr *) &clientAddress, &clientLen);

    client1Attributes.sourceSocket = connect1FileDescriptor;
    client1Attributes.destinationSocket = connect2FileDescriptor;

    client2Attributes.sourceSocket = connect2FileDescriptor;
    client2Attributes.destinationSocket = connect1FileDescriptor;

    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, clientInterconnect, &client1Attributes);
    pthread_create(&tid2, &attr, clientInterconnect, &client2Attributes);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    close(connect2FileDescriptor);
    close(connect1FileDescriptor);

    return 0;
}


void *clientInterconnect(void *sockfd)
{
    ssize_t n;
    char    buf[MAXLINE];
    int     errno;

    socketStruct_t *clientSockets = (socketStruct_t *) sockfd;

//again:
    memset(buf, 0, MAXLINE);
    while ((n= read(clientSockets->sourceSocket, buf, MAXLINE)) > 0)
    {
	    printf("%s\n", buf);
        write(clientSockets->destinationSocket, buf, n);
        memset(buf, 0, MAXLINE);
    }

    //if (n < 0 && errno == EINTR)
        //goto again;
    else if (n < 0)
        perror("Read Error");

    pthread_exit(0);
}
