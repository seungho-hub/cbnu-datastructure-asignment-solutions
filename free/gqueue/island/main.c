#include <stdio.h>
#include <stdlib.h>
#include "gqueue.h"

typedef struct
{
  int y;
  int x;
} cell_t;

int map[10][10];
int X, Y;

void printMap()
{
  for (int y = 0; y < Y; y++)
  {
    for (int x = 0; x < X; x++)
    {
      printf("%d ", map[y][x]);
    }
    printf("\n");
  }
}
int remove_an_island()
{
  int y, x;

  cell_t s;

  int found = 0;

  for (y = 0; y < Y; y++)
  {
    for (x = 0; x < X; x++)
    {
      if (map[y][x] == 1)
      {
        s.y = y;
        s.x = x;
        found = 1;
        break;
      }
    }
  }

  if (found == 0)
    return 0;

  gqueue_t *cells_to_erase = create_queue(X * Y, sizeof(cell_t));

  enqueue(cells_to_erase, &s);

  while (!is_empty(cells_to_erase))
  {
    cell_t c;
    dequeue(cells_to_erase, &c);

    // erase
    map[c.y][c.x] = 0;

    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {1, -1, 0, 0};

    for (int d = 0; d < 4; d++)
    {
      cell_t a;

      a.y = c.y + dy[d];
      a.x = c.x + dx[d];

      // 이어지는 land들을 모두 erase할 수 있도록 enqueue한다.
      if (0 <= a.y && a.y < Y && 0 <= a.x && a.x < X)
      {
        if (map[a.y][a.x] == 1)
        {
          enqueue(cells_to_erase, &a);
        }
      }
    }
  }

  delete_queue(cells_to_erase);

  return 1;
}

int main()
{
  FILE *fp = fopen("map.txt", "r");

  fscanf(fp, "%d %d", &X, &Y);

  for (int i = 0; i < Y; i++)
  {
    for (int j = 0; j < X; j++)
    {
      fscanf(fp, "%d", &(map[i][j]));
    }
  }
  fclose(fp);

  printMap();

  int countOfIsland = 0;

  while (remove_an_island())
  {
    countOfIsland++;
  }

  printMap();
  printf("%d\n", countOfIsland);

  return EXIT_SUCCESS;
}