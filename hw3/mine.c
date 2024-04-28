#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gqueue.c"
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdint.h>

int countNearMine(int x, int y);
int isValidUserInput(char *command, int x, int y);
int isValidCommand(char *command);
int isValidPosition(int x, int y);
int isOutOfMap(int x, int y);

typedef enum Color
{
	BLACK = 0,
	DARKBLUE = FOREGROUND_BLUE,
	DARKGREEN = FOREGROUND_GREEN,
	DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DARKRED = FOREGROUND_RED,
	DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	GRAY = FOREGROUND_INTENSITY,
	BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
} Color;

void showPrompt()
{
	printf("Enter the target cell and command to execute\n");
	printf("\n");
	printf("Commands:\n");
	printf("\t open\n");
	printf("\t flag\n");
	printf("\n");
	printf("\"command x y\" : ");
}

void SetColor(int8_t ForgC)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}

// cell state
typedef enum
{
	closed,
	marked,
	open
} cell_state;

// cell
typedef struct
{
	int mined;
	int num;
	cell_state state;
} cell_t;

// width, height, number of minded cell
int M, N, K;

cell_t board[16][16] = {{{0, 0, 0}}};

// { xOffset, yOffSet }
int nearOffset[8][2] = {
		// left
		{-1, 0},
		// right
		{1, 0},
		// bottom
		{0, 1},
		// top
		{0, -1},
		// left-top
		{-1, -1},
		// right-top
		{1, -1},
		// left-bottom
		{-1, 1},
		// right-bottom
		{1, 1}};

int defeated = false;

int countNearMine(int x, int y)
{
	int result = 0;

	for (int i = 0; i < sizeof(nearOffset) / sizeof(nearOffset[0]); i++)
	{
		int xOffset = nearOffset[i][0];
		int yOffset = nearOffset[i][1];

		// 범위를 넘어가지 않았으며
		if (!isOutOfMap(x + xOffset, y + yOffset))
		{
			// 그곳이 mineded cell이라면
			if (board[y + yOffset][x + xOffset].mined)
			{
				result++;
			}
		}
	}

	return result;
}

void load_board(char *filename)
{
	FILE *fp;

	if (!(fp = fopen(filename, "r")))
	{
		fprintf(stderr, "Fail to open %s\n", filename);
		exit(EXIT_FAILURE);
	}

	if (fscanf(fp, "%d %d %d", &M, &N, &K) != 3)
	{
		fprintf(stderr, "File format error\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < K; i++)
	{
		int x, y;
		if (fscanf(fp, "%d %d", &y, &x) != 2)
		{
			fprintf(stderr, "File format error\n");
			exit(EXIT_FAILURE);
		}

		board[y][x].mined = 1;
		board[y][x].num = 0;
		board[y][x].state = closed;
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (!board[i][j].mined)
			{
				board[i][j].num = countNearMine(j, i);
				board[i][j].state = closed;
			}
		}
	}

	fclose(fp);
}

char *currErrorMessage;

int isValidUserInput(char *command, int x, int y)
{
	return isValidCommand(command) && isValidPosition(x, y);
}
int isValidCommand(char *command)
{
	if (strcmp(command, "open") && strcmp(command, "flag"))
	{
		currErrorMessage = "invalid command";
		return false;
	}

	return true;
}
int isValidPosition(int x, int y)
{
	// out of map
	if (isOutOfMap(x, y))
	{
		currErrorMessage = "target cell is out of map";
		return false;
	}

	// already opened
	if (board[y][x].state == open)
	{
		currErrorMessage = "target cell already opened";
		return false;
	}

	return true;
}
int isOutOfMap(int x, int y)
{
	return ((x < 0 || x >= N) || (y < 0 || y >= M)) ? true : false;
}

int is_terminated()
{
	// mine을 click했으면 게임은 종료된다.
	if (defeated)
		return true;

	// 모든 cell이 열렸으면 게임은 종료된다.
	int allOpened = true;

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			// 닫힌 상태고, mined가 아니라면 열려야하 cell이 있는 것이다.
			if (board[y][x].state == closed && !board[y][x].mined)
			{
				allOpened = false;
			}
		}
	}

	if (allOpened)
		return true;

	// mine을 클릭하지도 않았고, 모든 cell이 열리지 않았으면 게임은 종료되지 않는다.
	return false;
}

void draw_board()
{
	printf("\n");
	int x, y;
	for (y = 0; y < M; y++)
	{
		for (x = 0; x < N; x++)
		{
			cell_t cell = board[y][x];

			switch (cell.state)
			{
			case (closed):
				SetColor(YELLOW);
				printf("#");
				break;
			case (marked):
				SetColor(MAGENTA);
				printf("F");
				break;
			case (open):
				if (cell.mined)
				{
					SetColor(RED);
					printf("X");
					break;
				}
				else
				{
					switch (cell.num)
					{
					case 0:
						SetColor(WHITE);
						printf(" ");
						break;
					case 1:
						SetColor(BLUE);
						printf("%d", cell.num);
						break;
					case 2:
						SetColor(GREEN);
						printf("%d", cell.num);
						break;
					case 3:
						SetColor(RED);
						printf("%d", cell.num);
						break;
					}
				}
			}
			printf("  ");
		}
		printf("\n");
		SetColor(WHITE);
	}
	printf("\n");
}

int isReserved(gqueue_t *q, int x, int y)
{
	int temp[2];

	for (int i = 0; i < get_size(q); i++)
	{
		get_elem(q, i, temp);

		if (temp[0] == x && temp[1] == y)
		{
			return true;
		}
	}

	return false;
}
// user가 선택한 cell을 open한다.
void userOpen(int x, int y)
{
	// mine이라면 종료한다.
	if (board[y][x].mined)
	{
		defeated = true;
		board[y][x].state = open;
		return;
	}

	/*
		mine이 아니고, 주변에 mine이 존재하지 않다면 펼친다.
		오직 open만한다.
	*/

	// target cell의 position을 enqueue한다.
	// target cell도 open되지 않은 상태다.
	gqueue_t *q = create_queue(M * N, sizeof(int[2]));
	int target[2] = {x, y};
	enqueue(q, &target);

	while (!is_empty(q))
	{
		int temp[2];
		dequeue(q, &temp);

		// 주변에 mine이 존재한다면 open하고 종료한다.
		if (board[temp[1]][temp[0]].num > 0)
		{
			board[temp[1]][temp[0]].state = open;
		}
		// 주변에 mine이 없다면, 주변 cell을 enqueue한다.
		else
		{
			board[temp[1]][temp[0]].state = open;
			for (int i = 0; i < sizeof(nearOffset) / sizeof(nearOffset[0]); i++)
			{
				int nearX = temp[0] + nearOffset[i][0];
				int nearY = temp[1] + nearOffset[i][1];
				int nearCell[2] = {nearX, nearY};

				// 이미 enqueue된 cell이라면 pass
				if (isReserved(q, nearX, nearY))
				{
					continue;
				}

				// 맵을 벗어난다면 pass
				if (isOutOfMap(nearX, nearY))
				{
					continue;
				}

				// 이미 open된 cell이라면 pass
				if (board[nearY][nearX].state == open)
				{
					continue;
				}

				enqueue(q, nearCell);
			}
		}
	}
}

void userFlag(int x, int y)
{
	board[y][x].state = marked;
}

void read_execute_userinput()
{
	/* FIXME*/
	char command[16];
	int x, y;
	showPrompt();
	scanf("%15s %d %d", command, &x, &y);

	if (!isValidUserInput(command, x, y))
	{
		SetColor(RED);
		printf("[ERROR] : invalid input : %s \n", currErrorMessage);
		SetColor(WHITE);
		return;
	}

	if (strcmp(command, "open") == 0)
	{
		userOpen(x, y);
	}
	else if (strcmp(command, "flag") == 0)
	{
		userFlag(x, y);
	}

	return;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Invalid command: give a filename of board arrangement.\n");
		return EXIT_FAILURE;
	}

	load_board(argv[1]);

	while (!is_terminated())
	{
		draw_board();
		read_execute_userinput();
	}

	if (defeated)
	{
		draw_board();
		printf("you defeated!");
	}
	return EXIT_SUCCESS;
}