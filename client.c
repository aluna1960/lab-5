#include "helpers.h"

#define PORT 6969
#define HOST "localhost"

int connectClient(int port, char * host);


/**
 * Connects a client to a server pointed by host at port.
 *
 * Creates a socket and tries to connect to the server with the given hostname at the given port.
 *
 * @param port port of the server.
 * @param host hostname of the server.
 * @return socket file descriptor on sucess, -1 if failed.
 */
int connectClient(int port, char *host){
    int cSocket;
    struct hostent *hp;
    struct sockaddr_in cAddr;

    if ((hp = gethostbyname(host)) == NULL)
        return -1; 
    bzero(&cAddr, sizeof(cAddr));
    cAddr.sin_family      = AF_INET;
    cAddr.sin_port        = htons(port);
    bcopy((char *) hp->h_addr, 
          (char *) &cAddr.sin_addr.s_addr, hp->h_length);
    /**
     * Your code here
     */
    memset(&(cAddr.sin_zero), '\0', 8);
    cSocket = socket(AF_INET, SOCK_STREAM, 0);
    int size = sizeof(cAddr);
    connect(cSocket, (struct sockaddr *)&cAddr, size);

    return cSocket;

}

int main(int argc, char**argv){

    char buf[1024];
    int cSocket;

    if ((argc != 2) || (strspn(argv[1],"0123456789") != strlen(argv[1]))){
        fprintf(stderr, "Usage: %s <Milliseconds>\n", argv[0]);
	    exit(1);
    }

    cSocket = connectClient(PORT,HOST);
    assert(cSocket>-1);

    printf("Number of milliseconds: %d",atoi(argv[1]));
    send(cSocket, argv[1], strlen(argv[1]), 0);
    recv(cSocket, buf, sizeof(buf), 0);
    assert(close(cSocket)==0);
    return 0;
}
