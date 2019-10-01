#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdio.h>
#include <stdlib.h>

//should make a standard  FIFO queue
//using a linkedlist

typedef struct stringNode{

  struct stringNode *next;
  char *str;

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
char *get(q *queue, int index);
#endif //LINKEDLIST_H
