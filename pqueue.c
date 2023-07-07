#include "pqueue.h"

Queue *create (int size) {
  Queue *q = (Queue *)malloc(sizeof(Queue));
  q->front = 0;
  q->back = 0;
  q->size = size;
  q->array = (point *)malloc(size * sizeof(point));
  return q;
}

void enqueue (Queue *q, point elem) {
  if (full(q)) {
    printf ("error: queue overflow!\n");
    exit(1);
  }	 
  q->array[q->back] = elem;
  q->back = (q->back + 1) % q->size;
}

point dequeue (Queue *q) {
  if (empty(q)) {
    printf ("error: queue underflow!\n");
    exit(1);
  }	  
  point e = q->array[q->front];
  q->front = (q->front + 1) % q->size;
  return e;
}

point front (Queue *q) {
  if (!empty(q)) 
    return q->array[q->front];	  
}

int empty (Queue *q) {
  return (q->front == q->back);	
}

int full (Queue *q) {
  return (q->front == ((q->back + 1) % q->size));	
}

void destroy (Queue *q) {
  free (q->array);
  free (q);
}

int getsize (Queue *q) {
  return q->size;
}
