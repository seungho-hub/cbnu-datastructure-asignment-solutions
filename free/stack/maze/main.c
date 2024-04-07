#include <stdio.h>
#include <stdlib.h>
#include "stack.c"

typedef enum
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	DONE
} dir;

// cell의 status
enum cell
{
	EMPTY,
	WALL,
	PATH
};

// width, height;
int X, Y;
int m[20][20];

void print_map()
{
	int i, j;

	for (i = 0; i < X + 2; i++)
		printf("XX");
	printf("\n");
	for (i = 0; i < Y; i++)
	{
		printf("XX");
		for (j = 0; j < X; j++)
		{
			if (m[i][j] == WALL)
				printf("XX");
			else if (m[i][j] == PATH)
				printf("::");
			else
				printf("  ");
		}
		printf("XX\n");
	}
	for (i = 0; i < X + 2; i++)
		printf("XX");
	printf("\n");
}

void load_map(char *filename)
{
	FILE *fp = fopen(filename, "r");
	fscanf(fp, "%d %d", &X, &Y);
	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{
			fscanf(fp, "%d", &(m[i][j]));
		}
	}
	fclose(fp);
}

// nx, ny : current location, visited를 추적하는 xs, ys에 존재하는지 확인한다.
int is_in_path(stack_t *xs, stack_t *ys, int nx, int ny)
{
	int i;
	for (i = 0; i < get_size(xs); i++)
	{
		int x, y;
		get_elem(xs, i, &x);
		get_elem(ys, i, &y);
		if (x == nx && y == ny)
			return 1;
	}

	return 0;
}

// solution function으로 main function에서 한번 call된다.
void search()
{
	// visited를 추적하기위한 stack 3개,

	// x의 좌표를 기록함
	stack_t *xs = create_stack(400);

	// y의 좌표를 기록함
	stack_t *ys = create_stack(400);

	// direction을 기록함
	stack_t *ds = create_stack(400);

	// start point를 push한다.
	push(xs, 0);
	push(ys, 0);

	// 얘는 왜 up을 push하는지는 아직 모르겠음.
	push(ds, UP);

	// visited가 존재하지 않을 때까지 반복한다.
	while (is_empty(xs) == 0)
	{
		// printStack(xs);
		// printStack(ys);
		// printStack(ds);
		// printf("\n");

		int x,
				y, d;
		pop(xs, &x);
		pop(ys, &y);
		pop(ds, &d);

		// 목적지라면, xs즉, 지나온 모든 cell의 index를 출력하고
		if (x == X - 1 && y == Y - 1)
		{
			for (int i = 0; i < get_size(xs); i++)
			{
				int x, y;
				get_elem(xs, i, &x);
				get_elem(ys, i, &y);
				printf("(%d,%d) ", y, x);
			}
			printf("\n");
			return;
		}

		// direction이 done이라면 skip한다.
		if (d == 4)
		{
			continue;
		}

		// x, y of next position
		int nx, ny;
		switch (d)
		{
			// up
		case UP:
		{
			nx = x;
			ny = y - 1;
			break;
		}
		// down
		case DOWN:
		{
			nx = x;
			ny = y + 1;
			break;
		}
		// left
		case LEFT:
		{
			nx = x - 1;
			ny = y;
			break;
		}
		// right
		case RIGHT:
		{
			nx = x + 1;
			ny = y;
			break;
		}
		}

		// 다음 방향을 박는다.
		push(xs, x);
		push(ys, y);
		push(ds, d + 1);

		// 현재 방향의 next cell이  b
		// nx, ny 즉, next position이 map을 벗어나지 않는 경우에만 xs, ys, ds에 push한다.
		if (0 <= nx && nx < X && 0 <= ny && ny < Y)
		{
			if (m[ny][nx] == 0)
			{
				// next position이 visited라면, skip
				if (is_in_path(xs, ys, nx, ny) == 0)
				{
					push(xs, nx);
					push(ys, ny);
					push(ds, 0);
				}
			}
		}
	}
}

int main()
{
	load_map("maze2.txt");
	print_map();

	search();

	return EXIT_SUCCESS;
}
