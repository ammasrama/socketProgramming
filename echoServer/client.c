/* Aum Amriteswaryai Namaha
 * Aum Ganeshaya Namaha
 *
 * File : client.c
 * Owner: Sriram Karunagaran
 * Desciption: networking echo client
 */

#include "global.h"

void str_cli(int);

int main(int argc, char *argv[])
{
    int       socketFileDescriptor;
    struct    sockaddr_in serverAddress;

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
    
    str_cli(socketFileDescriptor);
    
    return 0;
}
 
void str_cli(int sockfd)
{
    char        sendLine[MAXLINE], receiveLine[MAXLINE];
    int         n;

    while (fgets(sendLine, MAXLINE, stdin) != NULL)
    {	
        write(sockfd, sendLine, strlen(sendLine));

        memset(receiveLine, 0, MAXLINE);
        if ((n = read(sockfd, receiveLine, MAXLINE)) == 0)
            perror("server terminated prematurely");

        printf("%s\n", receiveLine);
        memset(sendLine, 0, MAXLINE);
    }
}
