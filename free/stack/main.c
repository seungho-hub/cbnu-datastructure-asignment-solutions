#include <stdio.h>
#include "stack.c"

int main()
{
  stack_t *s = create_stack(5);
  push(s, 4);
  push(s, 13);
  push(s, 15);
  push(s, 41);
  push(s, 52);

  int top;

  pop(s, &top);

  printf("%d\n", top);

  return 0;
}