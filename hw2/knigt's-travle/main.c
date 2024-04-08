#include <stdio.h>
#include "gstack.c"

int map[8][8] = {0};

int iToI(int i)
{
  return 8 - i;
}
int cToI(char c)
{
  return (int)c - 97;
}

char iToC(int i)
{
  return (char)(i + 97);
}

void printStack(gstack_t *xs, gstack_t *ys)
{
  for (int i = 0; i < xs->size; i++)
  {
    int x;
    int y;
    get_element(xs, i, &x);
    get_element(ys, i, &y);

    printf("%c%d ", iToC(y), iToI(x));
  }
  printf("\n");
}

int isInPath(gstack_t *xs, gstack_t *ys, int nx, int ny)
{
  int i;
  for (i = 0; i < get_size(xs); i++)
  {
    int x, y;
    get_element(xs, i, &x);
    get_element(ys, i, &y);
    if (x == nx && y == ny)
      return 1;
  }

  return 0;
}

void search(int fromX, int fromY, int toX, int toY)
{
  gstack_t *xs = create_stack(400, sizeof(int));
  gstack_t *ys = create_stack(400, sizeof(int));
  int moveOffset[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
  push(xs, &fromX);
  push(ys, &fromY);

  while (is_empty(xs) == 0)
  {
    int x;
    int y;

    printStack(xs, ys);
    printf("\n");

    pop(xs, &x);
    pop(ys, &y);

    // reach to destination
    if (x == toX || y == toY)
    {
      for (int i = 0; i < get_size(xs); i++)
      {
        int x, y;
      }

      return;
    }

    map[y][x] = 1;

    for (int i = 0; i < 8; i++)
    {
      int nextX = x + moveOffset[i][0];
      int nextY = y + moveOffset[i][1];

      // out of map
      if (nextX < 0 || nextY < 0 || nextX > 8 || nextY > 8 || map[nextY][nextX] == 1)
      {
        continue;
      }
      else
      {
        push(xs, &nextX);
        push(ys, &nextY);
      }
    }
  }

  delete_stack(xs);
  delete_stack(ys);
  return;
}

int main()
{
  int fromX, fromY, toX, toY;
  char tempFromX, tempToX;

  // printf("from : ");
  // scanf(" %c %d", &tempFromX, &fromY);
  // fromX = cToI(tempFromX);

  // printf("to : ");
  // scanf(" %c %d", &tempToX, &toY);
  // toX = cToI(tempToX);

  // printf("%d %d %d %d", fromX, fromY, toX, toY);
  // search(fromX, fromY, toX, toY);

  search(1, 1, 4, 6);
  return 0;
}