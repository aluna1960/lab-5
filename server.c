#include "helpers.h"
#include <pthread.h>
#include <semaphore.h>
#define PORT 6969



void *clientHandler(void *cSocket);
int startListen(int port);
pthread_t tid[10];
int* temp;
sem_t sem;

/**
 * Handle client request.
 *
 * Handles a client request by simulating an I/O request.
 * Reads the milliseconds passed by the client and sleeps for that ammount of time.
 * Upon waking up, writes back to the client to signal that the request is finished.
 *
 * @param pcSocket pointer (of type void to appease pthread create) to a clientsocket.
 * @return NULL.
 */
void *clientHandler(void *pcSocket){
    char buffer[1024];
    int rc;
    int cSocket = *(int *)pcSocket;
    if((rc = read((int)cSocket,buffer,sizeof(buffer)) == -1)){
        perror("Server read");
        close(cSocket);
        return NULL;
    }

    if (strspn(buffer,"0123456789") != strlen(buffer)){
        perror("Not a number");
        close(cSocket);
        return NULL;
    }

    long time = atol(buffer);
    printf("now serving client %d\n",cSocket);
    fflush(stdout);
    msleep(time);
    printf("Finshed working with client %d\n",cSocket);
    fflush(stdout);
    sprintf(buffer,"Finished %d",cSocket);
    write(cSocket,buffer,12);
    close(cSocket);
    memset(buffer,0,sizeof(buffer));
    return NULL;
}



/**
 * Create a server connection.
 *
 * Creates a socket and prepares it to start listening for incomming connections.
 *
 * @param port port that is prepared to listen to connections.
 * @return socket file descriptor on sucess, -1 if failed.
 */
int startListen(int port){
    int sSocket;
    struct sockaddr_in sAddr;
    
    bzero(&sAddr, sizeof(sAddr));
    sAddr.sin_family      = AF_INET;
    sAddr.sin_port        = htons(port);
    sAddr.sin_addr.s_addr = INADDR_ANY;
    
    
    // Your code here
    sSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(sSocket, (struct sockaddr *)&sAddr, sizeof(sAddr));
    listen(sSocket, 100);
    return sSocket;  
}

int qHandler(struct queue *q){

  while(1){
    sem_wait(&sem);
    temp = deQueue(q);
    sem_post(&sem);
    
    if(temp != NULL){
      clientHandler((void *)temp);
      free(temp);
    }
  }
}
  

int main(int argc, char**argv){
    
    int cSock;
    int cSize;
    struct sockaddr_in cAddr;
    int *sock;
    struct queue* queue = createQueue();
    sem_init(&sem, 0 ,1);
    
    printf("Opening socket\n");
    int sSocket = startListen(PORT);
    assert(sSocket>-1);

    int i;
    for(i = 0; i < 10; i++){
      pthread_create(&tid[i], NULL, qHandler, queue);
    }
    
    
    while(1){
      cSize = sizeof(cAddr);
      cSock = accept(sSocket, (struct sockaddr *)&cAddr, &cSize);
      
      
      sock = malloc(1);
      *sock = cSock;
      
      enQueue(queue, sock);
    }
    return 0;    
}
