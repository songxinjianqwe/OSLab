//
// Created by sinjinsong on 6/14/17.
//

#ifndef SLEEPINGBARBER_SEMPHORE_H
#define SLEEPINGBARBER_SEMPHORE_H

#include "proc.h"

#define MAX_QUEUE_SIZE 5

typedef struct queue{
    PROCESS processes[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int elements;
}Queue;

PUBLIC void init_queue(Queue * queue);
PUBLIC void enQueue(Queue * queue,PROCESS * p);
PUBLIC PROCESS * deQueue(Queue * queue);

typedef struct struct_semaphore {
    int value;
    Queue * queue;
}Semaphore;


#endif //SLEEPINGBARBER_SEMPHORE_H
