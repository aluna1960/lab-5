#include "helpers.h"

/**
 * Sleep with milliseconds.
 *
 * Sleep call for use with milliseconds.
 * Goes to sleep by the given amount of milliseconds.
 *
 * @param msec number of milliseconds.
 * @return 0 on sucess, -1 on failure.
 */
int msleep(long msec){
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

/**
 * Create a connections queue node.
 *
 * @param cSocket pointer to a client socket.
 * @return new node.
 */
struct qNode *newNode(int *cSocket){
    struct qNode *temp = (struct qNode*)malloc(sizeof(struct qNode));
    temp->cSocket = cSocket;
    temp->next = NULL;
    return temp;
}

/**
 * Create a connections queue.
 *
 * @return new connections queue.
 */
struct queue* createQueue(){
    struct queue* q = (struct queue*)malloc(sizeof(struct queue));
    q->head = q->tail = NULL;
    return q;
}

/**
 * Insert a new connection to a connections queue.
 *
 * @param q pointer to a connections q
 * @param cSocket pointer to a client socket.
 * @return nothing.
 */
void enQueue(struct queue *q, int *cSocket){
    struct qNode* temp = newNode(cSocket);
 
    if (q->tail == NULL) {
        q->head = q->tail = temp;
        return;
    }

    q->tail->next = temp;
    q->tail = temp;
}


/**
 * Retrive a connection from the connection q.
 *
 * @param q pointer to a connections q
 * @return first connection node in the queue, Null if queue is empty, 
 */
int* deQueue(struct queue* q){
    if (q->tail == NULL)
        return NULL;
    
    int *res = q->head->cSocket;
    struct qNode* temp = q->head;
    q->head = q->head->next;
 
    if (q->head == NULL)
        q->tail = NULL;
 
    free(temp);
    return res;
}
 