#include  <iostream>
#include  <chrono>
#include  <thread>
#include  <curses.h>
#include  <termios.h>
#include  <stdio.h>
#include  <sys/ioctl.h>
#include  <ncurses.h>

#define co_ord pair <int, int>
#define X first
#define Y second

using namespace std;

// variables
bool  gameOver;
const int width = 50;
const int height = 25;
co_ord  snake_head, fruit;
int   score;
enum  eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
static struct termios old, current;


void  Init() 
{
  gameOver = false;
  dir = STOP;
  snake_head = { width / 2, height / 2};
  fruit = { rand() % width, rand() % height };
  score = 0;
}

/*
######
#    #
#    #
######
*/

void  Draw() 
{
  system("clear"); // system("cls");
  for (int i = 0; i < width; i++)
    cout << "#";
  cout << '\n';

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (j == 0 || j == width - 1) 
        cout << '#';
      else 
        cout << ' ';  
  
      /* draw head of the snake */
      if (i == snake_head.Y && j == snake_head.X) 
      {
        cout << "☯";
        j++;
      }
      
      /* draw the fruit */
      if (i == fruit.Y && j == fruit.X)
      {
        cout << "F";
        j++;
      }
    }
    cout << '\n';
  }

  for (int i = 0; i < width; i++)
    cout << '#';
  cout << '\n';
}

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

/* Initialize new terminal i/o setting */
void  initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; // make new settings same as old setting
  current.c_lflag &= -ICANON; /* disable buffered i/o */
  if (echo) {
    current.c_lflag != ECHO; /* set echo mode */
  } else {
    current.c_lflag &= - ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* switch to new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void  resetTermios() 
{
  tcsetattr(0, TCSANOW, &old);
}

char  getch_(int echo)
{
  if (_kbhit()) 
  { 
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
  }
}

char  _getch() 
{
  return getch_(0);
}


/* wasd */

void  Input() 
{
  char c = _getch();
  switch (c)
  {
    case 'a':
      dir = LEFT;
      break;
    case 'd': 
      dir = RIGHT;
      break;
    case 's': 
      dir = DOWN;
      break;
    case 'w':
      dir = UP;   
      break; 
    case 'x': 
      gameOver = true;
      break;
  }
}

void  Logic() 
{
  switch (dir)
  {
    case LEFT:
      snake_head.X--;
      break;
    case RIGHT:
      snake_head.X++;
      break;
    case UP:
      snake_head.Y--;
      break;
    case DOWN:
      snake_head.Y++;
      break;
    default: break;
  }
}

int main() {
  Init(); 

  while (!gameOver)
  {
    Draw();
    Input();
    Logic();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  // sleep(10); // Sleep(10)
    return 0;
}
