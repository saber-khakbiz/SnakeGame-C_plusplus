/*******************************************************************
* Code written by saber Khakbiz
*******************************************************************/ 
#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <time.h>
#include <cstdlib>
#include <string>

using namespace std;
 

#define INIT_SNAKE_LENGTH 4
#define FOOD              1
#define WIN               60  //the number of hearts for victory
#define MAX               100
#define WIDTH             44
#define HEIGHT            15


#define EXIT -1 
#define RIGHT 0
#define UP    1
#define LEFT  2
#define DOWN  3

#define SNAKE  -1
#define NOTHING 0
#define WALL   -2

char food= 3;

const char Wall  = 219;
const char Head  = 1;
const char Body  = 43;

int input = RIGHT;	
int item = NOTHING;


static int dx[5] = { 1, 0, -1, 0 };
static int dy[5] = { 0, -1, 0, 1 };

 

 
void gotoxy(int column, int row)
{
	HANDLE hStdOut;
	COORD coord;
 
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;
 
	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(hStdOut, coord);
}
 
void clearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };
 
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;
 
	
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;
 
	
	if (!FillConsoleOutputCharacter(
		hStdOut,		
		(TCHAR) ' ',	
		cellCount,		
		homeCoords,		
		&count			
		)) return;
 
	
	if (!FillConsoleOutputAttribute(
		hStdOut,			
		csbi.wAttributes,	
		cellCount,			
		homeCoords,			
		&count				
		)) return;
 
	
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
 

int validPress(int input1, int input2)
{
	if (input1 == LEFT && input2 == RIGHT)
		return 1;
    else if (input1 == RIGHT && input2 == LEFT)
		return 1;
	else if (input1 == UP && input2 == DOWN)
		return 1;
	else if (input1 == DOWN && input2 == UP)
		return 1;
 
	return 0;
}

struct Coordinate
{
	int x, y;
};
 
class snake
{
private:
	int length;
	Coordinate body[WIDTH*HEIGHT];
	int direction;
	int ground[MAX][MAX];
	int foodCounter;
public:
	void initGround();
	void initSnake();
	void increaseSnakeLenght(int delay);
	void newFood();
	void firstDraw();
	int getFoodCounter();
};
 
void snake::initGround()
{
	
	int i, j;
	for (i = 0; i < WIDTH; i++)
		for (j = 0; j < HEIGHT; j++)
			ground[i][j] = 0;
 
	for (i = 0; i <= WIDTH + 1; i++)
	{
		
		ground[0][i] = WALL;
		ground[HEIGHT + 1][i] = WALL;
	}
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		
		ground[i][0] = WALL;
		ground[i][WIDTH + 1] = WALL;
	}
}
 
void snake::initSnake()
{
	length = INIT_SNAKE_LENGTH;		
	body[0].x = WIDTH / 2;
	body[0].y = HEIGHT / 2;
	direction = input;
	foodCounter = 0;
 
	int i;
	for (i = 1; i < length; i++)
	{
		body[i].x = body[i - 1].x - dx[direction];		
		body[i].y = body[i - 1].y - dy[direction];		
	}
	
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;
}
 
void snake:: increaseSnakeLenght(int delay)
{
	int i;
	Coordinate prev[WIDTH*HEIGHT];
	for (i = 0; i < length; i++)
	{
		prev[i].x = body[i].x;
		prev[i].y = body[i].y;
	}
 
	if (input != EXIT && ! validPress(direction, input))
		direction = input;
 
	body[0].x = prev[0].x + dx[direction];		
	body[0].y = prev[0].y + dy[direction];		
 
	if (ground[body[0].y][body[0].x] < NOTHING)
	{
		item = -1;
		return;
	}
 
	if (ground[body[0].y][body[0].x] == FOOD)
	{
		length++;		
		item = FOOD;
		Beep(400,30);
	}
	else
	{
		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);		
		cout << " ";						
	}
 
	for (i = 1; i < length; i++)
	{
		body[i].x = prev[i - 1].x;	
		body[i].y = prev[i - 1].y;	
	}
 
	gotoxy(body[1].x, body[1].y);
	cout << Body;					
	gotoxy(body[0].x, body[0].y);
	cout << Head;					
 
	
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;
 
	Sleep(delay);
 
	return;
}
 
void snake::newFood()
{
	int x, y;
	do
	{
		x = rand() % WIDTH + 1;
		y = rand() % HEIGHT + 1;
	} while (ground[y][x] != NOTHING);
 
	ground[y][x] = FOOD;
	foodCounter++;
	gotoxy(x, y);	
    cout << food;
    
}
 
void snake::firstDraw()
{
	clearScreen();
	int i, j;
	
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		for (j = 0; j <= WIDTH + 1; j++)
		{
			switch (ground[i][j])
			{
			case NOTHING:
				cout << " "; break;
			case WALL:
				if ((i == 0 && j == 0)
					|| (i == 0 && j == WIDTH + 1)
					|| (i == HEIGHT + 1 && j == 0)
					|| (i == HEIGHT + 1 && j == WIDTH + 1))
					cout << "+";    
				else
					if (j == 0 || j == WIDTH + 1)
					
						cout << Wall;    
					else
						cout << Wall;    
				break;
			case SNAKE:
				if (i == body[0].y && j == body[0].x)
					cout << Head;
				else
					cout << Body;
				break;
			default:				
				cout << food;	
			}
		}
		cout << endl;
	}
}
 
int snake::getFoodCounter()
{
	return foodCounter;
}
 
void movePlayer(void* id)
{
	do
	{
		int c = _getch();
		switch (c)
		{
		case 72: case 'w': input = UP   ; break;
		case 80: case 's': input = DOWN ; break;
		case 77: case 'd': input = RIGHT; break;
		case 75: case 'a': input = LEFT ; break;
		case 27:	    input = EXIT ; break;
		}
	} while (input != EXIT && item >= 0);
 
	_endthread();
	return;
}


int Speed (int win){
	if (0<=win && win<WIN/5) {
	  int Delay=210;
	  return Delay;
    }
	else if (WIN/5<=win && win<2*WIN/5) {
	  int Delay=180;
	  return Delay;	
	}
	else if (2*WIN/5<=win && win<3*WIN/5) {
	  int Delay=140;
	  return Delay;	
	}
	
	else if (3*WIN/5<=win && win<4*WIN/5) {
	  int Delay=110;
	  return Delay;	
	} 
	else if (4*WIN/5<=win && win<=WIN) {
	  int Delay=80;
	  return Delay;	
	}  
	
    return 0;	
}
	 
void Music_Win(){	
  Beep(329,100); 
  Beep(493,300); 
  Beep(698,300); 
  Beep(659,600);
  Sleep(2000);
}

void Music_Lose(){	
  Beep(320,150);
  Beep(300,200);
  Beep(280,600);
  Sleep(2000);
 
}


int main(int argc, char** argv)
{
	int win;
	int delay ;
	srand(time(NULL));
	snake Run;
	Run.initGround();
	Run.initSnake();
	Run.firstDraw();
	Run.newFood();
	_beginthread(movePlayer, 0, (void*)0);	

	do{
		win= Run.getFoodCounter();
		delay = Speed (win);
		Run. increaseSnakeLenght(delay);
		if (item == FOOD)
		Run.newFood();
			
	} while (item >= 0 && win<=WIN && input != EXIT);
         
    if (win<=WIN){
	   
	   gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	   cout << "GAME OVER :( ";
	   gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	   cout << "Your score is " << Run.getFoodCounter() - 1 << "!" << endl;
	   gotoxy(WIDTH / 2, HEIGHT / 2);
	   Music_Lose();
    }
    else{
       gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
       cout << "You win :)";
       gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	   cout << "Your score is " << Run.getFoodCounter() - 1 << "!" << endl;
	   Music_Win();
	   
	}
 
	
	return 0;
}






