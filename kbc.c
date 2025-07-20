#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // For _getch() and _kbhit() on Windows
#include <windows.h>
#include <time.h>
#define MAX_QES_LEN 300
#define MAX_OPT_LEN 100

const char *RED = "\033[1;31m";
const char *GREEN = "\033[1;32m";
const char *YELLOW = "\033[1;33m";
const char *BLUE = "\033[1;34m";
const char *PINK = "\033[1;35m";
const char *AQUA = "\033[1;36m";

const char *COLOUR_end = "\033[0m";

typedef struct
{
    char text[MAX_QES_LEN];
    char option[4][MAX_OPT_LEN];
    char correct_option;
    int timeout;
    int prize_money;
} Question;

int read_question(char *file_name, Question **question);
void print_formatted_ques(Question question);
void play_game(Question *question, int no_of_questions);
int use_lifeline(Question *question, int *lifeline);
char timed_input(int timeout_sec, int *timeout_flag);

// struct termios old_props;
// void reset_terminal_attr();
// void set_terminal_attr();

int main()
{
    srand(time(NULL));
    printf("\n\t\t %sCHALO KHELTEN HAIN KON  BNEGA CROREPATI !!!!%s", PINK, COLOUR_end);
    Question *question;
    int no_of_question = read_question("question.txt", &question);

    {
        play_game(question, no_of_question);
    }
    return 0;
}

// function to check that the ouestions are brought by the below function(displaying)
void print_formatted_ques(Question question)
{

    printf("\n\n%s%s%s", YELLOW, question.text, COLOUR_end);

    for (int i = 0; i < 4; i++)

    {
        if (question.option[i][0] != '\0')
        {
            printf("\n%s%c. %s%s", AQUA, ('A' + i), question.option[i], COLOUR_end);
        }
    }

    printf("\n\n%sHURRY YOU HAVE ONLY %d SECONDS!!!%s", YELLOW, question.timeout, COLOUR_end);
    printf("\n\n%s Enter your answer(A,B,C or D)or enter L for lifeline: %s\n", GREEN, COLOUR_end);
}
// function which will read the full file and bring the ouestions for us
int read_question(char *file_name, Question **question)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("\nunable to open the question bank.");
        exit(0);
    }

    char str[MAX_QES_LEN];
    int no_of_lines = 0;
    while (fgets(str, MAX_QES_LEN, file))
    {
        no_of_lines++;
    }
    int no_of_question = no_of_lines / 8;
    *question = (Question *)malloc(no_of_question * sizeof(Question));

    rewind(file);
    for (int i = 0; i < no_of_question; i++)
    {
        // Read question text
        fgets((*question)[i].text, MAX_QES_LEN, file);
        size_t len = strlen((*question)[i].text);
        if (len > 0 && (*question)[i].text[len - 1] == '\n')
        {
            (*question)[i].text[len - 1] = '\0';
        }

        for (int j = 0; j < 4; j++)
        {
            fgets((*question)[i].option[j], MAX_OPT_LEN, file);
            // Remove trailing newline
            size_t len_opt = strlen((*question)[i].option[j]);
            if (len_opt > 0 && (*question)[i].option[j][len_opt - 1] == '\n')
            {
                (*question)[i].option[j][len_opt - 1] = '\0';
            }
        }
        char option[10];
        fgets(option, 10, file);
        (*question)[i].correct_option = option[0];

        char timeout[10];
        fgets(timeout, 10, file);
        (*question)[i].timeout = atoi(timeout);

        char prize_money[10];
        fgets(prize_money, 10, file);
        (*question)[i].prize_money = atoi(prize_money);
    }
    fclose(file);
    return no_of_question;
}

void play_game(Question *question, int no_of_questions)
{
    int money_won = 0;
    int lifeline[] = {1, 1};

    
    for (int i = 0; i < no_of_questions; i++)
    {
        print_formatted_ques(question[i]); // printing the question

        int timeout_flag = 0;
char ch = timed_input(question[i].timeout, &timeout_flag);
if (timeout_flag) {
    printf("\n%sTIMEOUT!!! You took too long.%s\n", RED, COLOUR_end);
    _getch();  // Pause for user to see timeout message
    break;
}
ch = toupper(ch);
printf("%c", ch);

        if (ch == 'L')
        {
            int value = use_lifeline(&question[i], lifeline);
            if (value != 2)
            {
                i--;
            }
            continue;
        }
        if (ch == question[i].correct_option)
        {

            printf("\n%scorrect! %s", GREEN, COLOUR_end);
            money_won = question[i].prize_money;
            printf("%s\nyou have won Rs %d%s", BLUE, money_won, COLOUR_end);
        }
        else
        {
            printf("\n%sWrong answer correct answer is %c%s", RED, question[i].correct_option, COLOUR_end);
            break;
        }
    }
    printf("\n\n %s GAME OVER!!your total winnings are %d%s", BLUE, money_won, COLOUR_end);
}

int use_lifeline(Question *question, int *lifeline)
{
    printf("\n \n%sAvailable lifelines%s", PINK, COLOUR_end);
    if (lifeline[0])
    {
        printf("\n%s1. FIFTY-FIFTY (50/50)%s", AQUA, COLOUR_end);
    }
    if (lifeline[1])
    {
        printf("\n%s2. SKIP THE QUESTION %s", AQUA, COLOUR_end);
    }
    printf("\n%snow choose a lifeline OR 0 TO RETURN:%s", BLUE, COLOUR_end);
    char ch = (_getch());
    printf("%c", ch);

    switch (ch)
    {
    case '1':

        if (lifeline[0])
        {
            lifeline[0] = 0;
            int removed = 0;
            while (removed < 2)
            {
                int num = rand() % 4;
                if ((num + 'A') != question->correct_option && question->option[num][0] != '\0')
                {
                    question->option[num][0] = '\0';
                    removed++;
                }
            }

            return 1;
        }
        break;

    case '2':
        if (lifeline[1])
        {
            lifeline[1] = 0;
            return 2;
        }
        break;

    default:
        printf("\n%sReturning back to the question.%s", BLUE, COLOUR_end);
        return 0;
        break;
    }
}
char timed_input(int timeout_sec, int *timeout_flag) {
    time_t start = time(NULL);
    int remaining = timeout_sec;
    
    while (1) {
        if (_kbhit()) {
            return _getch();  // User pressed a key
        }

        int elapsed = (int)difftime(time(NULL), start);
        int new_remaining = timeout_sec - elapsed;

        if (new_remaining != remaining) {
            remaining = new_remaining;
            printf("\r%sTime remaining: %2d seconds...%s", YELLOW, remaining, COLOUR_end);
            fflush(stdout);
        }

        if (remaining <= 0) {
            *timeout_flag = 1;
            return '\0';  // Timeout occurred
        }

        Sleep(100);  // Small delay to reduce CPU usage
    }
}
