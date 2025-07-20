#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>          // For _getch() and _kbhit() on Windows
#include <windows.h>        // For Windows-specific functions like Sleep and cursor hiding
#else
#include <unistd.h>         // For sleep/usleep on Linux
#include <termios.h>        // For terminal attribute handling on Linux
#endif

#define height 20           // Game board height
#define width 60            // Game board width

// Enumeration to represent snake direction
enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};

// Global variables to manage game state
enum direction dir;         // Current direction of snake
int score;                  // Player's score
int tail_length;            // Length of the snake's tail
int fruit_x, fruit_y;       // Coordinates of the fruit
int head_x, head_y;         // Coordinates of the snake's head
int tail_x[100];            // X positions of tail segments
int tail_y[100];            // Y positions of tail segments

// Function declarations
void setup();
void clear_screen();
void reset_terminal_attr();
void set_terminal_attr();
void draw();
int input_availble();
void game_play();
void input();

// Clears the terminal screen (Windows/Linux)
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Terminal input settings for non-Windows systems
#ifdef _WIN32
void set_terminal_attr() {}
void reset_terminal_attr() {}
#else
struct termios old_props;   // Stores original terminal properties
void set_terminal_attr()
{
    tcgetattr(STDIN_FILENO, &old_props);           // Get current terminal attributes
    atexit(reset_terminal_attr);                   // Ensure reset on exit
    struct termios new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON);         // Disable echo and canonical input
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);  // Apply new settings
}
void reset_terminal_attr()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);  // Restore original settings
}
#endif

int main()
{
    srand(time(NULL));      // Seed the random generator for fruit placement
    set_terminal_attr();    // Prepare terminal for raw input
    hide_cursor();          // Optional: Hide cursor for better UX
    setup();                // Initialize game state

    while (1)
    {
        draw();             // Render game screen
        input();            // Handle user input
        game_play();        // Update game logic
        int sleep_time = 3000000 / (score != 0 ? score : 10); // Speed up as score increases
        usleep(sleep_time); // Frame delay
    }
    reset_terminal_attr();  // Reset terminal before exiting
    return 0;
}

#ifdef _WIN32
void input()
{
    if (input_availble())
    {
        char ch = _getch(); // Read user input without waiting
        switch (ch)
        {
        case 'a': dir = LEFT; break;    // Move left
        case 's': dir = DOWN; break;    // Move down
        case 'd': dir = RIGHT; break;   // Move right
        case 'w': dir = UP; break;      // Move up
        case 'x': exit(0); break;       // Exit game
        default: break;
        }
    }
}
#else
void input()
{
    if (input_availble())
    {
        char ch = getchar(); // Read character
        switch (ch)
        {
        case 'a': dir = LEFT; break;    // Move left
        case 's': dir = DOWN; break;    // Move down
        case 'd': dir = RIGHT; break;   // Move right
        case 'w': dir = UP; break;      // Move up
        case 'x': exit(0); break;       // Exit game
        default: break;
        }
    }
}
#endif


// Updates snake movement and checks for collisions
void game_play()
{
    // Shift tail segments forward (each follows the one before)
    for (int i = tail_length - 1; i > 0; i--)
    {
        tail_x[i] = tail_x[i - 1];
        tail_y[i] = tail_y[i - 1];
    }
    // Store head position as first tail segment
    tail_x[0] = head_x;
    tail_y[0] = head_y;

    // Move head based on direction
    switch (dir)
    {
    case UP: head_y--; break;
    case DOWN: head_y++; break;
    case LEFT: head_x--; break;
    case RIGHT: head_x++; break;
    case STOP: break;  // No movement
    }

    // Wrap around if snake hits wall (appears on opposite side)
    if (head_x < 0) head_x = width - 1;
    else if (head_x >= width) head_x = 0;
    if (head_y < 0) head_y = height - 1;
    else if (head_y >= height) head_y = 0;

    // Check if snake hits its own tail (game over)
    for (int i = 0; i < tail_length; i++)
    {
        if (tail_x[i] == head_x && tail_y[i] == head_y)
        {
            printf("You have hit your tail,[game over]\n");
            exit(0);
        }
    }

    // Check if fruit is eaten
    if (head_x == fruit_x && head_y == fruit_y)
    {
        score += 10;                     // Increase score
        tail_length++;                   // Increase tail
        fruit_x = rand() % width;        // Place new fruit
        fruit_y = rand() % height;
    }
}

// Check if user input is available (cross-platform)
#ifdef _WIN32
int input_availble() { return _kbhit(); }
#else
int input_availble()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv); // Check if data is available to read
}
#endif

// Draw the game field
void draw()
{
    clear_screen(); // Clear console output
    printf("\t\t**Welcome to the snake game**\n");

    // Top border
    for (int i = 0; i < width + 2; i++) printf("#");

    // Game area row by row
    for (int i = 0; i < height; i++)
    {
        printf("\n#"); // Left wall
        for (int j = 0; j < width; j++)
        {
            if (i == head_y && j == head_x)
                printf("O"); // Snake head
            else if (i == fruit_y && j == fruit_x)
                printf("F"); // Fruit
            else
            {
                int tail_found = 0;
                for (int k = 0; k < tail_length; k++)
                {
                    if (tail_x[k] == j && tail_y[k] == i)
                    {
                        printf("o"); // Snake tail
                        tail_found = 1;
                        break;
                    }
                }
                if (!tail_found) printf(" "); // Empty space
            }
        }
        printf("#"); // Right wall
    }

    // Bottom border
    printf("\n");
    for (int i = 0; i < width + 2; i++) printf("#");

    printf("\nSCORE: %d \n\n", score); // Display score
}

// Initialize game state at the start
void setup()
{
    head_x = width / 2;                // Start head in the center
    head_y = height / 2;
    fruit_x = rand() % width;          // Random fruit position
    fruit_y = rand() % height;
    dir = LEFT;                        // Initial direction
    score = 0;                         // Initial score
    tail_length = 0;                   // Initial tail size
}

// Optional: Hide cursor on Windows console
#ifdef _WIN32
void hide_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
#endif
