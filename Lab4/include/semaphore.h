//
// Created by sinjinsong on 6/14/17.
//

#ifndef SLEEPINGBARBER_SEMAPHORE_H
#define SLEEPINGBARBER_SEMAPHORE_H
#include "proc.h"

#define MAX_QUEUE_SIZE 5
#define MAX_SLEEP_TIME 2147483647
#define CHAIRS 1

typedef struct queue{
    PROCESS* processes[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int elements;
}Queue;


typedef struct struct_semaphore {
    int value;
    Queue * queue;
}Semaphore;

PUBLIC void enQueue(Queue * queue,PROCESS * p);
PUBLIC PROCESS * deQueue(Queue * queue);
PUBLIC void initSemaphore(Semaphore * semaphore,int value,Queue * queue );

#endif //SLEEPINGBARBER_SEMAPHORE_H
