#include <stdio.h>
#include "queue.c"

int main()
{
  int capacity = 10;

  gqueue_t *queue = create_queue(capacity);

  for (int i = 0; i < capacity; i++)
  {
    int n = i + 2;

    enqueue(queue, &n);
  }

  printQueue(queue);

  int n;

  dequeue(queue, &n);

  printf("%d\n", n);
  printQueue(queue);

  enqueue(queue, &n);
  printQueue(queue);

  return 0;
}