/* Aum Amriteswaryai Namaha
 * Aum Ganeshaya Namaha
 *
 * File : client.c
 * Owner: Sriram Karunagaran
 * Desciption: networking echo client
 */

#include "global.h"

//void str_cli(int);

void *getData(void *);
void *displayData(void *);

int main(int argc, char *argv[])
{
    int       socketFileDescriptor;
    struct    sockaddr_in serverAddress;

    pthread_t tid1, tid2; // two threads
    pthread_attr_t attr; // thread attributes

    if (argc < 2)
    {
        perror("usage: client <ipaddress>");
    }

    /* STEP1 - call the socket function */
    /* family - IPv4 socket (AF_INET),
     * type   - SOCK_STREAM, stream socket
     * protocol - IPPROTO_TCP - TCP protocol
     */
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port   = htons(SERV_PORT);

    /* STEP2 - convert the user entered address into network address format
     * using pton function.
     */
	inet_pton(AF_INET, argv[1], &serverAddress.sin_addr);

    /* STEP3 - connect to the server provding the ip address and the port.
     */
    connect(socketFileDescriptor, (struct sockaddr*) &serverAddress, sizeof(serverAddress));

    pthread_attr_init(&attr);
    pthread_create(&tid1, &attr, getData, &socketFileDescriptor);
    pthread_create(&tid2, &attr, displayData, &socketFileDescriptor);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    close(socketFileDescriptor);
    
    return 0;
}
 
void *getData(void * sockfd)
{
    char sendLine[MAXLINE];

    int *clientSockfd = (int *) sockfd;

    memset(sendLine, 0, MAXLINE);
    while (fgets(sendLine, MAXLINE, stdin) != NULL)
    {	
        write(*clientSockfd, sendLine, strlen(sendLine));
        memset(sendLine, 0, MAXLINE);
    }

    pthread_exit(0);
}

void *displayData(void *  sockfd)
{
    char  receiveLine[MAXLINE];
    int   n;

    int *clientSockfd = (int *) sockfd;

    memset(receiveLine, 0, MAXLINE);
    
    while ((n = read(*clientSockfd, receiveLine, MAXLINE)) != 0)
    {
        if (n  == -1)
           break;

        printf("%s\n", receiveLine);
        memset(receiveLine, 0, MAXLINE);
    }

    pthread_exit(0);
}
