#include <stdio.h>
#include<stdlib.h>
#include<time.h>


#define BOARD_SIZE 3
#define X 'X'
#define O  'O'

typedef struct{
    int playerwon;
    int computerwon;
    int draw;
}SCORE;


int difficulty;
int game_mode;
SCORE score ={.playerwon=0,.computerwon=0,.draw=0};//starting scores

void input_difficulty();
void clear_screen();
void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int check_win(char board[BOARD_SIZE][BOARD_SIZE],char player);
int check_draw(char board[BOARD_SIZE][BOARD_SIZE]);
void player_move(char board[BOARD_SIZE][BOARD_SIZE],char player);
void computer_move(char board[BOARD_SIZE][BOARD_SIZE]);
int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE],int row,int col);
void play_game_pvc();
void play_game_pvp();

int main()
{
    srand(time(NULL));
    int choice;
    int last_mode;
    do{
        do
        {
            printf("\n\n***WELCOME TO TIC TAC TOE THE GAME***");
            printf("\n-------------------------------------------\n");
            printf("Select your game mode:\n");
            printf("ENTER 1 FOR PLAYER VS PLAYER MODE\n");
            printf("ENTER 2 FOR PLAYER VS COMPUTER MODE \n");
            printf("Now Enter your choice\n");
            scanf("%d", &game_mode);
            
        } while (game_mode!=1 && game_mode!=2);
        if(game_mode!=last_mode){
            score.playerwon=0;
            score.computerwon=0;
            score.draw=0;
        }
        if(game_mode==2){
        input_difficulty();
        play_game_pvc();
        }else{
            play_game_pvp();
        }
        last_mode=game_mode;
        
    printf("\nplay again?(press 1 to replay 0 to exit)\n");
    scanf("%d", &choice);
    }while (choice==1);

        printf("THANKS FOR PLAYING\n");
        printf("Bye bye!!!\n");
        printf("\n\n\n");
    
    
    return 0;
    }
   
    void clear_screen(){
       // #ifdef _Win32
        system("cls");
     //  #else 
      // system("clear");
      // #endif
    }

void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
    clear_screen();
    if(game_mode==2){
    printf("\n\nplayer = %d,computer = %d , draws = %d ",score.playerwon,score.computerwon,score.draw);
    printf("\nTIC-TAC-TOE\n");
    }else{printf("\n\nplayer 1 = %d,player 2 = %d , draws = %d ",score.playerwon,score.computerwon,score.draw);
    printf("\nTIC-TAC-TOE\n");}
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("\n");
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf(" %c ",board[i][j]);
            if(j<BOARD_SIZE-1){
                printf("|");
            }
        }
        if(i<BOARD_SIZE-1){
            printf("\n---+---+---");
        }
    }
   printf("\n\n"); 
}

int check_win(char board[BOARD_SIZE][BOARD_SIZE],char player){

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i][0]==player
            && board[i][1]==player
              && board[i][2]==player)
        {
        return 1;
        }
        if (board[0][i]==player 
            && board[1][i]==player
                && board[2][i]==player   )
        {
            return 1;
        }
    }
        if (board[0][0]==player && board[1][1]==player && board[2][2]==player
           ||board[2][0]==player && board[1][1]==player && board[0][2]==player){
            return 1;
        }
    return 0;}

    int check_draw(char board[BOARD_SIZE][BOARD_SIZE]){
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if(board[i][j]==' '){
                    return 0;
                }
            }
            
        }
       return 1; 
    }
    int is_valid_move(char board[BOARD_SIZE][BOARD_SIZE],int row,int col){
    return !(row<0||col<0
            ||row>2||col>2
            ||board[row][col]!=' ');
  }
  void player_move(char board[BOARD_SIZE][BOARD_SIZE], char player) {

    int row, col;
    do {
        printf("\nPlayer %c's turn. Enter row and column (1-3): ", player);
        scanf("%d %d", &row, &col);
        row--; col--;
    } while (!is_valid_move(board, row, col));
    board[row][col] = player;  
    }
    void input_difficulty(){
        while(1){
    
    printf("\nHi i am computer i am here to play Tic-Tac-TOE with you. \n");
    printf("Plz Choose the difficulty level at which you want to play:\n");
    printf("1.HUMAN (standard)\n");
    printf("2.GOD (IMPOSSIBLE)\n");
    printf("make you choice:");
    scanf("%d", &difficulty);
    if(difficulty!=1 && difficulty!=2){
        printf("Invalid choice plz enter:(1/2)\n");
    }else{
        break;
    }
    }
}
           
    void computer_move(char board[BOARD_SIZE][BOARD_SIZE]){
        //check every block
        //1.immediate win
        
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if(board[i][j]==' '){
                    board[i][j]= O;
                    if (check_win(board,O))
                    {
                        return;
                    }
                    board[i][j]=' ';
                }
            }
            
        }
        //2.for immediate block
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if(board[i][j]==' '){
                    board[i][j]= X;
                    if (check_win(board,X)){
                    board[i][j]= O;
                        return;
                    }
                    board[i][j]=' ';
                }
            }
            
        }
       //god mode
       if (difficulty==2)
       {
        //3.play centre if available
        if(board[1][1]==' '){
            board[1][1]=O;        
             return;}
     //4.PLAY CORNER IF AVAILABLE
     int corners[4][2] = {{0,0},{0,2},{2,0},{2,2}};
        for (int i = 0; i < 4; i++) {
            int x = corners[i][0], y = corners[i][1];
            if (board[x][y] == ' ') {
                board[x][y] = O;
                return;
            }
        }
      
       }
       
        //FIRST EMPTY PLACE
      for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if(board[i][j]==' '){
                    board[i][j]= O;
                       return;  
                    }
            }
            
        } 
    }

    void play_game_pvp(){

        char board [BOARD_SIZE][BOARD_SIZE]={
        {' ',' ',' '},
        {' ',' ',' '},
        {' ',' ',' '}
    };
    char current_player = rand()%2 == 0 ?X:O;

    print_board(board);
    while (1){
        print_board(board);
      player_move(board,current_player);
      if(check_win(board,current_player)){    
        score.playerwon++;
        print_board(board);
        printf("\n congratulations You won!!!");
        break;
      }
    if (check_draw(board))
    {
        score.draw++;
       print_board(board);
        printf("\nIt's a draw!!!");
        break;
        }
        current_player = (current_player==X)?O:X;
     }
  }
  void play_game_pvc() {
    char board[BOARD_SIZE][BOARD_SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    char current_player = rand() % 2 == 0 ? X : O;

    while (1) {
        print_board(board);
        if (current_player == X) {
            player_move(board,X);
            if (check_win(board, X)) {
                print_board(board);
                printf("\nCongratulations, you won!\n");
                score.playerwon++;
                break;
            }
        } else {
            computer_move(board);
            if (check_win(board, O)) {
                print_board(board);
                printf("\nComputer wins!\n");
                score.computerwon++;
                break;
            }
        }

        if (check_draw(board)) {
             score.draw++;
            print_board(board);
            printf("\nIt's a draw!\n");
           
            break;
        }

        current_player = (current_player == X) ? O : X;
    }
}

  