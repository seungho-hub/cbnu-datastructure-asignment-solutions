#include <stdio.h>
#include <Stdlib.h>

typedef struct
{
  int *buffer;
  int capacity;
  int top;
} stack_t;

stack_t *
create_stack(int capacity)
{
  stack_t *stack = (stack_t *)malloc(sizeof(stack_t));

  stack->capacity = capacity;
  stack->buffer = (int *)malloc(capacity * sizeof(int));
  stack->top = 0;

  return stack;
}

int is_empty(stack_t *stack)
{
  return stack->top == 0;
};

int is_full(stack_t *stack)
{
  return stack->top == stack->capacity;
}

int top(stack_t *s, int *elem)
{
  if (is_empty(s))
  {
    return 0;
  }

  *elem = s->buffer[s->top];

  return 1;
}

void delete_stack(stack_t *s)
{
  free(s->buffer);
  free(s);
}

int push(stack_t *s, int elem)
{
  if (is_full(s))
  {
    return 0;
  }

  s->buffer[s->top] = elem;
  s->top++;

  return 1;
};

int pop(stack_t *s, int *elem)
{
  if (is_empty(s))
  {
    return 0;
  }

  *elem = s->buffer[s->top - 1];
  s->top -= 1;
  return 1;
}

int get_size(stack_t *stack)
{
  return stack->top;
}

int get_elem(stack_t *s, int index, int *elem)
{
  if (index < 0 || s->top <= index)
  {
    return 0;
  }

  return s->buffer[index];
}