#include <stdio.h>
#include "gstack.c"

int map[8][8] = {0};

typedef enum
{
  UP_RIGHT,
  UP_LEFT,
  DOWN_RIGHT,
  DOWN_LEFT,
  LEFT_UP,
  LEFT_DOWN,
  RIGHT_UP,
  RIGHT_DOWN,
  DONE
} dir;

int firstDirection = UP_RIGHT;

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
  gstack_t *xs = create_stack(64, sizeof(int));
  gstack_t *ys = create_stack(64, sizeof(int));
  gstack_t *ds = create_stack(64, sizeof(int));

  int moveOffset[8][2] = {
      {1, -2}, {-1, -2}, // UP RIGHT, LEFT
      {1, 2},
      {-1, 2}, // DOWN RIGHT, LEFT
      {-2, -1},
      {-2, 1}, // LEFT UP, DOWN
      {2, 1},
      {2, -1} // RIGHT UP, DOWN
  };

  push(xs, &fromX);
  push(ys, &fromY);
  push(ds, &firstDirection);

  while (is_empty(xs) == 0)
  {
    int x, y, d;

    printStack(xs, ys);
    printf("%d", d);
    printf("\n");

    pop(xs, &x);
    pop(ys, &y);
    pop(ds, &d);

    // reach to destination
    if (x == toX && y == toY)
    {
      printf("reached!");
      for (int i = 0; i < get_size(xs); i++)
      {
        int logX, logY;
        get_element(xs, i, &logX);
        get_element(ys, i, &logY);

        printf("%c%d ", iToC(logX), iToI(logY));
      }

      printf("\n");
      return;
    }

    if (d == DONE)
    {
      continue;
    }

    int nextX = x + moveOffset[d][0];
    int nextY = y + moveOffset[d][1];
    int nextD = d + 1;

    push(xs, &x);
    push(ys, &y);
    push(ds, &nextD);

    // out of map && in path
    if (nextX < 0 || nextY < 0 || nextX >= 8 || nextY >= 8 || isInPath(xs, ys, nextX, nextY))
    {
      continue;
    }
    else
    {
      push(xs, &nextX);
      push(ys, &nextY);
      push(ds, &firstDirection);
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

  printf("from : ");
  scanf(" %c %d", &tempFromX, &fromY);
  fromX = cToI(tempFromX);

  printf("to : ");
  scanf(" %c %d", &tempToX, &toY);
  toX = cToI(tempToX);

  printf("%d %d %d %d", fromX, fromY, toX, toY);
  search(fromX, iToI(fromY), toX, iToI(toY));

  return 0;
}