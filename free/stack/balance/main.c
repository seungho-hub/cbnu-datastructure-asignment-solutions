#include <stdio.h>
#include "chstack.c"

// string이 balance한지 여부를 return한다.
// string에는 open, close이외의 다른 character가 포함되어있으면 matching을 완료하고 나서 open이 남아있는지 확인하는 과정에서 false가 return된다.
int is_balanced(char *buf)
{
  stack_t *left = create_stack(256);

  for (int i = 0; i < buf[i] != '\0'; i++)
  {
    // 열린 기호일 경우, stack에 push
    if (buf[i] == '(' || buf[i] == '[' || buf[i] == '{' || buf[i] == '<')
    {
      push(left, buf[i]);
    }
    // 닫힌 기호일 경우, stack의 top과 mathcing되어야한다.
    else if (buf[i] == ')' || buf[i] == ']' || buf[i] == '}' || buf[i] == '>')
    {
      int r;
      char top;
      r = pop(left, &top);

      // empty일 경우 close는 있는데 open이 더이상 없느 상황 unbalance
      if (r == 0)
      {
        delete_stack(left);
        return 0;
      }

      /*
        matching되지 않는다면 unbalance.
      */
      if (buf[i] == ')' && top != '(')
      {
        delete_stack(left);
        return 0;
      }
      if (buf[i] == '}' && top != '{')
      {
        delete_stack(left);
        return 0;
      }
      if (buf[i] == ']' && top != '[')
      {
        delete_stack(left);
        return 0;
      }
      if (buf[i] == '>' && top != '<')
      {
        delete_stack(left);
        return 0;
      }
    }
  }

  // matching이 문제없이 종료되었다.
  // 하지만 open이 남아있다면, unbalance
  if (get_size(left))
  {
    delete_stack(left);
    return 0;
  }

  // 여기까지 도달했다면 balance함.
  delete_stack(left);
  return 1;
}

int main()
{
  char buf[256];

  scanf("%255s", buf);

  if (is_balanced(buf))
    printf("balanced.\n");
  else
    printf("not balanced\n");

  return 0;
}