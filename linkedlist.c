#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct stringNode{

  struct stringNode *next;
  char *element;

}str;


typedef struct Queue{
  str *head;
  str *tail;
  int size;
}q;

_Bool QisEmpty(q *queue);
str *new_node();
void enqueue(q *queue, char *element);
char  *dequeue(q *queue);
q *initialize_queue();

q *initialize_queue(){
  
q *queue = (q*)malloc(sizeof(q));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  return queue;
}

str *new_node(char *element){
  
  str *temp = (str*)malloc(sizeof(str));
  temp->element = element;
  temp->next = NULL;
  return temp;
  
}

void enqueue(q *queue, char *element){

  str *temp = new_node(element);
  
  if (queue->tail == NULL || queue->size == 0){
    queue->head = temp;
    queue->tail = temp;
  }else{

    queue->tail->next = temp;
    queue->tail = temp;

  }
  queue->size++;
}                                            

char *dequeue(q *queue){
 
  if (queue->size == 0){
    return NULL;
  }else{

    str *temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    return temp->element;
  }
  }

  _Bool QisEmpty(q *queue){
    if (queue->size == 0){
      return 1;
    }else{
      return 0;
    }
}
