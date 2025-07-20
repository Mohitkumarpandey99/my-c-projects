#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#define height 20
#define width 60

enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};

enum direction dir;
int score;
int tail_length;
int fruit_x, fruit_y; // x row y col
int head_x, head_y;
int tail_x[100];
int tail_y[100];

void setup();
void clear_screen();
void reset_terminal_attr();
void set_terminal_attr();
void draw();
int input_availble();
void game_play();
void input();

void clear_screen()
{

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#ifdef _WIN32
void set_terminal_attr()
{
    // No direct equivalent needed for basic input in Windows
    // You can use _getch() from <conio.h> for non-echoed, non-blocking input
}

void reset_terminal_attr()
{
    // No action needed for Windows
}
#else
struct termios old_props;
void set_terminal_attr()
{
    tcgetattr(STDIN_FILENO, &old_props);
    atexit(reset_terminal_attr);
    struct termios new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);
}

void reset_terminal_attr()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
}
#endif

int main()
{
    srand(time(NULL));
    set_terminal_attr();
    hide_cursor();
    setup();

    while (1)
    {
        draw();
        input();
        game_play();
        // Sleep(1);
        int sleep_time = 3000000 / (score != 0 ? score : 10); // inc speed
        usleep(sleep_time);
    }
    reset_terminal_attr();
    return 0;
}
#ifdef _WIN32
void input()
{
    // setting our controls
    if (input_availble())
    {
        char ch = _getch();
        switch (ch)
        {
        case 'a':
            dir = LEFT;
            break;

        case 's':
            dir = DOWN;
            break;

        case 'd':
            dir = RIGHT;
            break;

        case 'w':
            dir = UP;
            break;

        case 'x':
            exit(0);
            break;

        default:
            break;
        }
    }
#else
if (input_availble())
{
    char ch = _getch();
    switch (ch)
    {
    case 'a':
        dir = LEFT;
        break;

    case 's':
        dir = DOWN;
        break;

    case 'd':
        dir = RIGHT;
        break;

    case 'w':
        dir = UP;
        break;

    case 'x':
        exit(0);
        break;

    default:
        break;
    }
}
#endif
}

// initialzing our snake moving directions
void game_play()
{

    for (int i = tail_length - 1; i > 0; i--)
    {

        tail_x[i] = tail_x[i - 1];
        tail_y[i] = tail_y[i - 1];
    }
    tail_x[0] = head_x;
    tail_y[0] = head_y;

    switch (dir)
    {

    case UP:
        head_y--;
        break;

    case DOWN:
        head_y++;
        break;

    case LEFT:
        head_x--;
        break;

    case RIGHT:
        head_x++;
        break;

    case STOP:
        // DO NOTHING
        break;
    }
    if (head_x < 0)  //handling the snake going out of the boundaries
    {
        head_x = width - 1;
    }
    else if (head_x >= width)
    {
        head_x = 0;
    }
    if (head_y < 0)
    {
        head_y = height - 1;
    }
    else if (head_y >= height)
    {
        head_y = 0;
    }
    for (int i = 0; i < tail_length; i++)
    {
        if (tail_x[i] == head_x && tail_y[i] == head_y)
        {
            printf("You have hit your tail,[game over]\n");
            exit(0);
        }
    }

    if (head_x == fruit_x && head_y == fruit_y) //eating fruit and generating it again at eandom location
    {
        score += 10;
        tail_length++;
        fruit_x = rand() % width;
        fruit_y = rand() % height;
    }
}

// checking is inpput available from the keyboard

#ifdef _WIN32
int input_availble()
{
    return _kbhit();
}
#else
int input_availble()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
#endif

// drawing our box
void draw()
{
    clear_screen();
    printf("\t\t**Welcome to the snake game**");
    printf("\n");
    for (int i = 0; i < width + 2; i++)
    {
        printf("#");
    }

    for (int i = 0; i < height; i++)
    {
        printf("\n#");
        for (int j = 0; j < width; j++)
        {
            if (i == head_y && j == head_x)
            {
                printf("O");
            }
            else if (i == fruit_y && j == fruit_x)
            {
                printf("F");
            }
            else
            {
                int tail_found = 0;
                for (int k = 0; k < tail_length; k++)
                {
                    if (tail_x[k] == j && tail_y[k] == i)
                    {
                        printf("o");
                        tail_found = 1;
                        break;
                    }
                }
                if (!tail_found)
                {
                    printf(" ");
                }
            }
        }
        printf("#");
    }

    printf("\n");
    for (int i = 0; i < width + 2; i++)
    {
        printf("#");
    }
    printf("\nSCORE: %d \n\n", score);
}

void setup()
{
    // initializing the starting points of our snake and fruit(initialized randomly)
    head_x = width / 2;
    head_y = height / 2;
    fruit_x = rand() % width;
    fruit_y = rand() % height;
    dir = LEFT;
    score = 0; 
    tail_length = 0;
}

#ifdef _WIN32
void hide_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // Hide cursor
    SetConsoleCursorInfo(consoleHandle, &info);
}
#endif
