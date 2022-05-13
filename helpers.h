#ifndef __HELPERS_h__
#define __HELPERS_h__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/tcp.h>
#include <netinet/in.h>

typedef struct sockaddr sockaddr_t;

struct qNode {
    int *cSocket;
    struct qNode *next;
    
};

struct queue
{
    struct qNode *head, *tail;
};

int msleep(long msec);
struct qNode *newNode(int *cSocket);
struct queue *createQueue();
void enQueue(struct queue *q, int *cSocket);
int *deQueue(struct queue* q);

#endif // __HELPERS_h__