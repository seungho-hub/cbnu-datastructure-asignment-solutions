#include <stdio.h>
#include <stdlib.h>
#include "gstack.c"

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

// map의 width, height
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
int is_in_path(gstack_t *xs, gstack_t *ys, int nx, int ny)
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

// solution function으로 main function에서 단 한번 call된다.
void search()
{
	// dfs를 위한 3개의 stack x, y와 direction을 저장하게되고,

	// x의 좌표를 기록함
	gstack_t *xs = create_stack(400, sizeof(int));

	// y의 좌표를 기록함
	gstack_t *ys = create_stack(400, sizeof(int));

	// direction을 기록함
	gstack_t *ds = create_stack(400, sizeof(int));

	// start position
	int start_x = 0, start_y = 0;
	int start_d = UP;

	// start position과 direction을 push한다.
	push(xs, &start_x);
	push(ys, &start_y);
	push(ds, &start_d);

	while (is_empty(xs) == 0)
	{
		int x,
				y, d;

		// dfs
		pop(xs, &x);
		pop(ys, &y);
		pop(ds, &d);

		// dest에 도착했다면 지나온 모든 cell의 index를 출력하고 함수를 종료
		if (x == X - 1 && y == Y - 1)
		{
			m[y][x] = PATH;
			printf("path : ");
			for (int i = 0; i < get_size(xs); i++)
			{
				int x, y;
				get_element(xs, i, &x);
				get_element(ys, i, &y);

				m[y][x] = PATH;

				printf("(%d,%d) ", y, x);

				if (i < get_size(xs) - 1)
				{
					printf("-> ");
				}
			}
			printf("\n");
			return;
		}

		// 모든 방향에 대해 stack에 push 되어있다면, dfs를
		if (d == 4)
		{
			continue;
		}

		// next node의 position을
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

		// 다음 방향을 stack에 넣는다.
		push(xs, &x);
		push(ys, &y);

		int nextDirection = d + 1;
		push(ds, &nextDirection);

		// nx, ny 즉, next position이 map을 벗어나지 않는 경우에만 xs, ys, ds에 push한다.
		if (0 <= nx && nx < X && 0 <= ny && ny < Y)
		{
			if (m[ny][nx] == 0)
			{
				// next position이 visited라면, skip
				if (is_in_path(xs, ys, nx, ny) == 0)
				{
					int up = UP;
					push(xs, &nx);
					push(ys, &ny);
					push(ds, &up);
				}
			}
		}
	}
}

int main()
{
	load_map("maze1.txt");
	print_map();
	printf("\n");

	search();
	printf("\n");

	print_map();

	return EXIT_SUCCESS;
}