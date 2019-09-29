#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct jobNode{

struct jobNode *next;
  int timestamp;
  int job_ID;

}job;


typedef struct Queue{
  job *head;
  job *tail;
  int size;
}q;

_Bool QisEmpty(q *queue);
job *new_node();
void enqueue(q *queue, int timestamp, int job_ID);
job  *dequeue(q *queue);
q *initialize_queue();

q *initialize_queue(){
  
q *queue = (q*)malloc(sizeof(q));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  return queue;
}

job *new_node(int timestamp, int job_ID){
  
  job *temp = (job*)malloc(sizeof(job));
  temp->timestamp = timestamp;
  temp->job_ID = job_ID;
  temp->next = NULL;
  return temp;
  
}

void enqueue(q *queue, int timestamp, int job_ID){

  job *temp = new_node(timestamp, job_ID);
  
  if (queue->tail == NULL || queue->size == 0){
    queue->head = temp;
    queue->tail = temp;
  }else{

    queue->tail->next = temp;
    queue->tail = temp;

  }
  queue->size++;
}                                            

job *dequeue(q *queue){
 
  if (queue->size == 0){
    return NULL;
  }else{

    job *temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    return temp;
  }
  }

  _Bool QisEmpty(q *queue){
    if (queue->size == 0){
      return 1;
    }else{
      return 0;
    }
}
