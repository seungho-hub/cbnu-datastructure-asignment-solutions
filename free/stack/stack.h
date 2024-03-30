typedef struct
{
  int *buffer;
  int capacity;
  int top;
} stack_t;

stack_t *
create_stack(int capacity);

void delete_stack(stack_t *s);

int push(stack_t *s, int elem);

int pop(stack_t *s, int *elem);

int is_empty(stack_t *s);

int is_full(stack_t *s);

int get_size(stack_t *s);
