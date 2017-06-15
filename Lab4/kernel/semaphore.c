//
// Created by sinjinsong on 6/14/17.
//
#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "semaphore.h"

PRIVATE void initQueue(Queue * queue){
    queue->front = 0;
    queue->rear = 0;
    queue->elements = 0;
}

PUBLIC void enQueue(Queue * queue,PROCESS * p){
    if(queue->elements == MAX_QUEUE_SIZE){
        disp_str_with_color("Queue Full! Can not enQueue",NORMAL_CHAR_COLOR);
        return ;
    }
    queue->processes[queue->rear] = p;
    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE ;
    queue->elements++;
}

PUBLIC PROCESS * deQueue(Queue * queue){
    if(queue->elements == 0){
        return 0;
    }
    PROCESS * result = queue->processes[queue->front];
    queue->processes[queue->front] = 0;
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE ;
    queue->elements --;
    return result;
}

PUBLIC void initSemaphore(Semaphore * semaphore,int value,Queue * queue ){
    initQueue(queue);
    semaphore->value = value;
    semaphore->queue = queue;
}
