#include <stdio.h>
#include <string.h>
#include <unistd.h>
// #include <termios.h> // for controlling terminal behavior
#include <conio.h> // for getch() on Windows

// Constants
#define Max_users 10              // Maximum number of users allowed
#define credential_length 30      // Maximum length of username and password

// Structure to store username and password for each user
typedef struct {
    char username[credential_length];
    char password[credential_length];
} User;

// Global array of users
User users[Max_users];
int user_count = 0;   // Current number of registered users

// Function declarations
void register_user();                      // To register a new user
int login_user();                           // To login a user (returns user index)
void fix_fgets_input(char* string);          // To remove '\n' from fgets input
void input_credential(char* username, char* password);   // To input username and masked password

int main() {
    int option;
    int user_index;

    while (1) { // Infinite loop for menu
        printf("\nWelcome to User Management System!\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter option: ");
        
        scanf("%d", &option);   // Taking user input
        getchar();              // Consuming the leftover newline character from scanf

        switch (option) {
            case 1:
                register_user();
                break;
            case 2:
                user_index = login_user();
                if (user_index >= 0) { // Successful login
                    printf("\nLogin successful! Welcome, %s.\n", users[user_index].username);
                } else { // Failed login
                    printf("\nLogin failed! Wrong username or password.\n\n");
                }
                break;
            case 3:
                printf("Exiting program.\n");
                return 0; // Exit the program
            default:
                printf("\nInvalid option chosen! Please try again.\n\n");
                break;
        }
    }

    return 0;
}

// Function to register a new user
void register_user() {
    if (user_count == Max_users) { // Check if user limit reached
        printf("Maximum users reached! Only %d users are allowed.\n", Max_users);
        return;
    }

    int new_index = user_count;  // New user will be stored at current count index
    printf("\nRegister new user!\n");

    input_credential(users[new_index].username, users[new_index].password); // Take username and password input
    user_count++; // Increase user count after successful registration
    printf("\nRegistration successful!\n");
}

// Function to login user
int login_user() {
    char username[credential_length];
    char password[credential_length];

    input_credential(username, password); // Input login credentials

    // Compare input credentials with stored user data
    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            return i; // Credentials matched, return user index
        }
    }

    return -1; // No match found
}

void input_credential(char* username, char* password) {
    printf("\nEnter username: ");
    fgets(username, credential_length, stdin); // Input username
    fix_fgets_input(username);                 // Remove '\n' from username if any

    printf("Enter password (masking enabled):\n");
    fflush(stdout); // Force the above print to show immediately

    char ch;
    int i = 0;

    // Read password character by character using getch()
    while (1) {
        ch = getch();
        if (ch == '\r' || ch == '\n') { // Enter key pressed
            break;
        } else if (ch == '\b' || ch == 127) { // Backspace pressed
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < credential_length - 1 && ch != 0) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate the password
    printf("\n");
}

// Function to remove the newline '\n' character from a string read by fgets
void fix_fgets_input(char* string) {
    int index = strcspn(string, "\n"); // Find position of '\n'
    string[index] = '\0';              // Replace '\n' with '\0'
}

/*
Step-by-Step Plan to Make This User Management Program


---

Step 1: Decide what features you want

Registration of users (with username and password).

Login with username and password.

Password masking (hide password input with *).

Limit number of users (say, 10 users maximum).

Use simple menu (register, login, exit).



---

Step 2: Plan data structures

You need a structure (struct) to store username and password together.

An array of that structure to store multiple users.

Example:

typedef struct {
    char username[30];
    char password[30];
} User;

User users[10];  // Maximum 10 users



---

Step 3: Implement the main() function

Use an infinite loop to display the menu:

1 → Register

2 → Login

3 → Exit


Take user input for choice.

Based on the choice, call the appropriate function.



---

Step 4: Implement user registration

Function: void register_user()

Steps:

Check if user limit reached.

If not, ask for username and password.

Save username and password to the next index in array.

Increment user count.




---

Step 5: Implement user login

Function: int login_user()

Steps:

Ask for username and password.

Loop through the registered users:

If match found → return user index (login successful).

Else → after loop, return -1 (login failed).





---

Step 6: Implement input with password masking

Function: void input_credential(char* username, char* password)

Steps:

Use fgets() to read username.

Use special terminal settings (termios) to:

Turn off ECHO and ICANON modes temporarily.

Read password character-by-character.

Print * instead of actual characters.

Handle backspace properly.


Restore the terminal settings after password input.




---

Step 7: Fix fgets() newline issue

Function: void fix_fgets_input(char* string)

Steps:

When you use fgets(), it stores the \n character at the end.

Remove that \n manually using strcspn() function.




---

Step 8: Handle small improvements

After scanf("%d", &option), always call getchar() to consume the newline.

Print clear success/failure messages.

Properly format outputs for good user experience.



---

Step 9: Testing

Try registering a few users.

Try logging in with correct and wrong credentials.

Try exceeding maximum users.

Check password masking behavior.



---

Step 10: (Optional Future Improvements)

Save users into a file (so they don't vanish after program exits).

Allow deleting a user.

Allow updating password.

Add password strength checks (e.g., minimum length, use of symbols).

Add admin panel (for viewing all users).



---

Summary Diagram

Main Menu
  |
  |-- Register User
  |      |
  |      |-- Input username
  |      |-- Input password (with masking)
  |      |-- Save to array
  |
  |-- Login User
         |
         |-- Input username
         |-- Input password (with masking)
         |-- Match with existing users
                 |-- Success or failure message
                 
        */
         