#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

int row(int cell)
{
  return cell / 8;
}

int col(int cell)
{
  return cell % 8;
}

// queen을 모두 배치시킬 수 있는지 여부를 확인한다.
int is_feasible(stack_t *queens)
{
  int board[8][8];
  int c, r;

  for (r = 0; r < 8; r++)
  {
    for (c = 0; c < 8; c++)
    {
      board[r][c] = 0;
    }
  }

  // quuen을 꺼내 모두 배치시키고 공격범위를 마킹한다.
  // 배치시킬 때 다른 queen의 공격범위라면, false를 return한다.
  // queen을 나타내는 value는 64개의 board cell에서의 위치다 즉, 9번째라면 [1][1], row(), col()로 array에서의 index가 계산된다.
  for (int i = 0; i < get_size(queens); i++)
  {
    int cell;
    get_elem(queens, i, &cell);

    int r = row(cell);
    int c = col(cell);

    if (board[r][c] != 0)
    {
      return 0;
    }

    int x, y;
    for (y = 0; y < 8; y++)
    {
      board[y][c] = 1;
    }
    for (x = 0; x < 8; x++)
    {
      board[r][x] = 1;
    }

    y = r + 1;
    x = c + 1;
    while (0 <= y && y < 8 && 0 <= x && x < 8)
    {
      board[y][x] = 1;
      y += 1;
      x += 1;
    }

    y = r + 1;
    x = c - 1;
    while (0 <= y && y < 8 && 0 <= x && x < 8)
    {
      board[y][x] = 1;
      y += 1;
      x -= 1;
    }

    y = r - 1;
    x = c + 1;
    while (0 <= y && y < 8 && 0 <= x && x < 8)
    {
      board[y][x] = 1;
      y -= 1;
      x += 1;
    }

    y = r - 1;
    x = c - 1;
    while (0 <= y && y < 8 && 0 <= x && x < 8)
    {
      board[y][x] = 1;
      y -= 1;
      x -= 1;
    }
  }

  return 1;
}

// 8개의 queen을 배치시킬 수 있는 모든 경우를 찾는다.
int main()
{
  // 배치시킬 queen은 8개다.
  stack_t *queens = create_stack(8);

  // [0][0] 에 queen을 먼저 꽂는다.
  push(queens, 0);

  // stack이 빌 때까지 반복한다.
  // i번째 cell부터 maximum을 8로 queen을 계속 추가한다.
  while (!is_empty(queens))
  {
    // last queen을 가져온다 (pop 아님)
    int latest_queen;
    top(queens, &latest_queen);

    // lastest queen이 64 즉, i번째 cell부터 모든 경우의 7개의 queen을 배치한 경우를 모두 탐색했다면
    if (latest_queen == 64)
    {
      // pop
      pop(queens, &latest_queen);

      // pop을 한후에 stack이 비어있지 않다면, 아직 모든 경우를 탐색한 것은 아님.
      if (!is_empty(queens))
      {
        pop(queens, &latest_queen);
        push(queens, latest_queen + 1);
      }

      // stack이 비었다면 모~든 경우의 탐색이 종료된 것.
      else
      {
        // main loop를 깬다.
        break;
      }

      continue;
    }

    // 모든 경우의 cell이 아직 탐색되지 않은 경우 여기까지 도달한다.

    /*
      아래는 3가지 경우가 있다.
      1. queen이 8개 이며 feasible한 경우.
        => lastest queen을 pop하고 next cell에 queen을 위치시켜 탐색을 진행한다. (search goal이기 때문에 queen들의 위치를 출력해준다.)
      2. feasible 하지만, 8개가 안되는 경우
        => lastest의 next cell에 queen을 추가로 위치시켜 탐색을 진행한다. (1, 2 둘 중 하나의 경우에 도달하기 위해)
      3. feasible하지 않은 경우
        => lastest queen을 pop하고 next cell에 queen을 위치시켜 탐색을 진행한다.
    */
    // 현재 queen의 배치가 가능하다면
    if (is_feasible(queens))
    {
      // 1 에 해당하는 경우, feasible하며 queen이 8개인 경우
      if (get_size(queens) == 8)
      {
        // 출력한다.
        for (int i = 0; i < 8; i++)
        {
          int queen;
          get_elem(queens, i, &queen);
          printf("%d ", queen);
        }
        printf("\n");
        // return 0 ;

        // 마지막 queen을 pop하고, 다음 위치의 경우의 queen을 push한다. (64번째인 경우는 위에서 걸려졌다.)
        int lastest_queen;
        // 8개 이상은 찾을 필요가 없기 때문에 pop하고 lastest의 next cell을 push
        pop(queens, &latest_queen);
        push(queens, latest_queen + 1);
      }

      // 2에 해당하는 경우, feasible하지만, 8개가 안되는 경우
      // 8개가 아니라면 8개가 될 때까지 일단 꽂아야된다.
      // 현재 위치를 가져와 다음 위치의 경우의 queen을 push한다.
      else
      {
        int latest_queen;
        // 그저 다음 위치에 push하기 위해 가져오는 것이다.
        top(queens, &latest_queen);
        push(queens, latest_queen + 1);
      }
    }
    // 2 에 해당하는 경우, feasible하지 않은경우
    // 현재 queen의 배치가 가능하지 않다면 뒤의 모든 경우도 안되기 때문에 현재 queen을 pop하고 lastest의 next cell을 push
    else
    {
      int lastest_queen;
      pop(queens, &latest_queen);
      push(queens, latest_queen + 1);
    }
  }
  delete_stack(queens);

  return EXIT_FAILURE;
}
