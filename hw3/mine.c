#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gqueue.h"

// ANSI Color Code
#define RESET "\033[0m"
#define BLACK "\033[90m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN "\033[96m"
#define WHITE "\033[97m"
#define DARKWHITE "\033[37m"

#define COMMAND_OPEN "open"
#define COMMAND_MARK "mark"
#define COMMAND_CHECK "check"
#define COMMAND_JUMP "jump"
#define COMMAND_GIVEUP "giveup"

void setColor(const char *colorCode);
int countNearMine(int x, int y);
void load_board(char *filename);
int isValidCommand(char *command);
int isValidPosition(int x, int y);
int isOutOfMap(int x, int y);
int is_terminated();
void draw_board();
int isReserved(gqueue_t *q, int x, int y);
void userOpen(int x, int y);
void userMark(int x, int y);
void userJump();
void printError(char *message);
int countOfIncorrectMark();
void openAll();
void read_execute_userinput();

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

// 남은 check 횟수
int checkRemain = 2;

// 남은 jump 횟수
int jumpRemain = 1;

int defeated = 0;

// error message
char *currErrorMessage;

// ANSI code로 output의 색상을 변경한다.
void setColor(const char *colorCode)
{
	printf("%s", colorCode);
}

// 인접한 cell까지의 offset이다. { xOffset, yOffSet }
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

// 인접한 mined cell의 개수를 반환한다.
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

// prompt를 보여준다.
void showPrompt()
{
	printf("\n");
	printf("Commands:\n");
	printf("\t open | open x y\n");
	printf("\t\t You can open the cell, be careful of mines\n");
	printf("\n");
	printf("\t mark | mark x y\n");
	printf("\t\t You can mark or unmark cell\n");
	printf("\n");
	printf("\t check (remain : %d)\n", checkRemain);
	printf("\t\t You can see the number of incorrect marks\n");
	printf("\n");
	printf("\t jump (remain : %d)\n", jumpRemain);
	printf("\t\t Open a cell that is not a mine\n");
	printf("\n");
	printf("\t giveup\n");
	printf("\t\t giveup game\n");
	printf("\n");
	printf("> ");
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

	// mined cell을 file로부터 입력받는다.
	for (int i = 0; i < K; i++)
	{
		int x, y;
		if (fscanf(fp, "%d %d", &y, &x) != 2)
		{
			fprintf(stderr, "File format error\n");
			exit(EXIT_FAILURE);
		}

		// mined로 표기한다.
		// cell의 state는 default인 0 즉, closed 상태다.
		board[y][x].mined = 1;
	}

	// non-mined cell에 인접한 mine의 수를 표기한다.
	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < M; x++)
		{
			// mined가 아니라면
			if (!board[y][x].mined)
			{
				// 인접한 mine의 개수를 저장한다.
				board[y][x].num = countNearMine(x, y);
			}
		}
	}

	fclose(fp);
}

// 유효한 command 즉,
int isValidCommand(char *command)
{
	if (strcmp(command, COMMAND_OPEN) && strcmp(command, COMMAND_MARK) && strcmp(command, COMMAND_CHECK) && strcmp(command, COMMAND_JUMP) && strcmp(command, COMMAND_GIVEUP))
	{
		currErrorMessage = "invalid command";
		return 0;
	}

	return 1;
}
int isValidPosition(int x, int y)
{
	// out of map
	if (isOutOfMap(x, y))
	{
		currErrorMessage = "target cell is out of map";
		return 0;
	}

	// 이미 열려있다면 아무런 작동도하지 못한다.
	if (board[y][x].state == open)
	{
		currErrorMessage = "target cell already opened";
		return 0;
	}

	return 1;
}
int isOutOfMap(int x, int y)
{
	return ((x < 0 || x >= N) || (y < 0 || y >= M)) ? 1 : 0;
}

int is_terminated()
{
	// mine을 click했으면 게임은 종료된다.
	if (defeated)
		return 1;

	// 모든 cell이 열렸으면 게임은 종료된다.
	int allOpened = 1;

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			// 닫힌 상태고, mined가 아니라면 열려야하 cell이 있는 것이다.
			if (board[y][x].state == closed && !board[y][x].mined)
			{
				allOpened = 0;
			}
		}
	}

	if (allOpened)
		return 1;

	// mine을 클릭하지도 않았고, 모든 cell이 열리지 않았으면 게임은 종료되지 않는다.
	return 0;
}

void draw_board()
{
	printf("\n");
	printf("\n");
	int x, y;

	printf("   ");
	setColor(DARKWHITE);
	// x축의 index를 보여준다.
	for (int i = 0; i < N; i++)
	{
		printf("%d  ", i);
	}

	printf("\n");

	for (y = 0; y < M; y++)
	{ // y 축의 index를 보여준다.
		setColor(DARKWHITE);
		printf("%d  ", y);

		//
		for (x = 0; x < N; x++)
		{
			cell_t cell = board[y][x];

			switch (cell.state)
			{
				// close 라면 #
			case (closed):
				setColor(YELLOW);
				printf("#");
				break;
				// marked라면 M
			case (marked):
				setColor(MAGENTA);
				printf("M");
				break;
				// open이라면
			case (open):
				// mineded cell인경우 X
				if (cell.mined)
				{
					setColor(RED);
					printf("X");
					break;
				}
				// non-minded cell이라면 인접한 mine의 개수를 출력
				else
				{
					// 숫자에 따라 색상 변경하기 위해
					switch (cell.num)
					{
					case 0:
						setColor(WHITE);
						printf(" ");
						break;
					case 1:
						setColor(BLUE);
						printf("%d", cell.num);
						break;
					case 2:
						setColor(GREEN);
						printf("%d", cell.num);
						break;
					case 3:
						setColor(RED);
						printf("%d", cell.num);
						break;
					}
				}
			}
			printf("  ");
		}
		printf("\n");
	}
	setColor(RESET);
	printf("\n");
}

// 이미 탐색이 예약되어있다면
int isReserved(gqueue_t *q, int x, int y)
{
	int temp[2];

	for (int i = 0; i < get_size(q); i++)
	{
		get_elem(q, i, temp);

		if (temp[0] == x && temp[1] == y)
		{
			return 1;
		}
	}

	return 0;
}

// cell을 open한다.
void userOpen(int x, int y)
{
	// mine이라면 종료한다.
	if (board[y][x].mined)
	{
		defeated = 1;
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

				// 이미 탐색이 예정된 cell이거나, 맵을 벗어났거나, 이미 열려있는 상태라면 패스한다.
				if (isReserved(q, nearX, nearY) || isOutOfMap(nearX, nearY) || board[nearY][nearX].state == open)
					continue;

				enqueue(q, nearCell);
			}
		}
	}

	delete_queue(q);
}

void userMark(int x, int y)
{
	// 열려있는 상태의 cell은 앞단계의 position validation check에서 block된다.
	// mark되어있는 상태라면 close로 되돌리고, marked되어있지 않은 상태라면 closed상태다. marking 해준다.
	board[y][x].state = board[y][x].state == marked ? closed : marked;
}

// non-minded cell 하나를 open한다.
void userJump()
{
	int randomX;
	int randomY;
	cell_t randomCell;

	// random value를 위한 seed
	srand(time(NULL));

	// mined이거나, 이미 열렸거나, 닫힌 cell이 아닌경우 적절한 cell이 지정될떄까지 반복한다.
	while (randomCell.mined || randomCell.state == open || randomCell.state != closed)
	{
		randomY = rand() % M;
		randomX = rand() % N;

		randomCell = board[randomY][randomX];
	}

	userOpen(randomX, randomY);
}

// 빨간 텍스트를 출력한다.
// 많이 사용되어서 빼놨다.
void printError(char *message)
{
	setColor(RED);
	printf("%s", message);
	setColor(WHITE);
}

//
int countOfIncorrectMark()
{
	int result = 0;

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			// mark되었지만 mined가 아니라면
			if (board[y][x].state == marked && !board[y][x].mined)
			{
				result++;
			}
		}
	}

	return result;
}

// cell들을 모두연다.
// 게임이 종료되었을 떄만 사용된다.
void openAll()
{
	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			board[y][x].state = open;
		}
	}
}

void read_execute_userinput()
{
	/* FIXME*/
	char command[16];
	int x, y;
	showPrompt();

	// command를 먼저 입력받는다.
	// command와 argument들을 동시에 입력해도 된다.
	scanf("%15s", command);

	// 유효한 command가 아니라면
	if (!isValidCommand(command))
	{
		printError("[ERROR] : invalid command");
		return;
	}

	if (strcmp(command, COMMAND_OPEN) == 0)
	{	
		printf("x y : ");
		scanf("%d %d", &x, &y);
		if (!isValidPosition(x, y))
		{
			printError("[ERROR] : invalid position");
			return;
		}
		userOpen(x, y);
	}
	else if (strcmp(command, COMMAND_MARK) == 0)
	{
		printf("x y : ");
		scanf("%d %d", &x, &y);
		if (!isValidPosition(x, y))
		{
			printError("[ERROR] : invalid position");
			return;
		}
		userMark(x, y);
	}
	else if (strcmp(command, COMMAND_CHECK) == 0)
	{
		if (checkRemain > 0)
		{
			setColor(CYAN);
			printf("\n\n------ There are (%d) incorrect mark(s) ------\n", countOfIncorrectMark());
			setColor(WHITE);
			checkRemain--;
		}
		else
		{
			printError("You have exhausted all your opportunities to check");
		}
	}
	else if (strcmp(command, COMMAND_JUMP) == 0)
	{
		if (jumpRemain > 0)
		{
			userJump();
			jumpRemain--;
		}
		else
		{
			printError("You have exhausted all your opportunities to jump");
		}
	}
	else if (strcmp(command, COMMAND_GIVEUP) == 0)
	{
		defeated = 1;
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

	// 게임이 어떻게 종료되었든 모든 cell을 열어 보여준다.
	openAll();
	draw_board();

	// 패배한 경우라면
	if (defeated)
	{
		setColor(RED);
		printf("you defeated!");
	}
	// 클리어한 경우라면
	else
	{
		setColor(GREEN);
		printf("clear!");
	}

	return EXIT_SUCCESS;
}