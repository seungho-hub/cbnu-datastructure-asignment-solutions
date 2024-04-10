#include <stdio.h>
#include "cqueue.c"

int main()
{
  int capacity = 10;

  gqueue_t *queue = create_queue(capacity);

  for (int i = 0; i < capacity; i++)
  {
    int n = i;

    enqueue(queue, &n);
  }
  printBuffer(queue);
  printQueue(queue);

  for (int i = 0; i < 5; i++)
  {
    int n;

    dequeue(queue, &n);

    printf("pop : %d\n", queue->buffer[i]);
  }
  printBuffer(queue);
  printQueue(queue);

  for (int i = 0; i < 5; i++)
  {
    int n = i;

    enqueue(queue, &n);
  }
  printBuffer(queue);
  printQueue(queue);

  return 0;
}