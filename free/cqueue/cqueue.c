#include <stdlib.h>

typedef struct
{
  int *buffer;

  int front;
  int rear;

  int capacity;
  int size;
} gqueue_t;

gqueue_t *create_queue(int capacity)
{
  gqueue_t *queue = (gqueue_t *)malloc(sizeof(gqueue_t));

  queue->capacity = capacity;
  queue->front = 0;
  queue->rear = 0;
  queue->size = 0;
  queue->buffer = (int *)calloc(capacity, sizeof(int));

  return queue;
}

void printBuffer(gqueue_t *queue)
{
  printf("buffer : ");
  for (int i = 0; i < queue->size; i++)
  {
    printf("%d ", queue->buffer[i]);
  }
  printf("\n");
}

void printQueue(gqueue_t *queue)
{
  printf("queue : ");
  for (int i = 0; i < queue->size; i++)
  {
    printf("%d ", queue->buffer[(queue->front + i) % queue->capacity]);
  }
  printf("\n");
}

int is_empty(gqueue_t *queue)
{
  return queue->size == 0;
}

int is_full(gqueue_t *queue)
{
  return queue->size == queue->capacity;
}

void delete_queue(gqueue_t *queue)
{
  free(queue->buffer);
  free(queue);
}

int enqueue(gqueue_t *queue, int *elem)
{
  if (is_full(queue))
    return 0;

  queue->buffer[queue->rear] = *elem;
  queue->rear = (queue->rear + 1) % queue->capacity;

  queue->size++;

  return 1;
}

int dequeue(gqueue_t *queue, int *elem)
{
  if (is_empty(queue))
    return 0;

  *elem = queue->buffer[queue->front];

  queue->front = (queue->front + 1) % queue->capacity;
  queue->size--;

  return 1;
}

int front(gqueue_t *queue, int *elem)
{
  if (is_empty(queue))
    return 0;

  *elem = queue->buffer[queue->front];

  return 1;
}
int get_size(gqueue_t *queue)
{
  return queue->size;
}

int get_elem(gqueue_t *queue, int index, int *elem)
{
  if (index > queue->size)
  {
    return 0;
  }

  int i = (queue->front + index) % queue->capacity;

  *elem = queue->buffer[i];

  return 0;
}