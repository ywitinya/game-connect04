/*****
 * Project 02: Grid game
 * COSC 208, introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//constant vars 
const int ROWS = 6;
const int COLUMNS = 7;
const char PLAYER1 = 'X'; 
const char PLAYER2 = 'O'; 

//Create Struct w/ yellow chips & blue chips 
struct players {
    char player1; 
    char player2; 
    char activePlayer; 
    char Winner; 

}; 



//First Create 2D Array 
char** createArr(){
    char **gridArr = malloc(sizeof(char*) * ROWS); 
    for (int i = 0; i < ROWS; i++){
        gridArr[i] = malloc(sizeof(char) * COLUMNS); 

        for (int j = 0; j < COLUMNS; j++){
            gridArr[i][j] = '_';  
        }
        
    }
    return gridArr; 
}


int diagonalRight (char** grid, struct players *gamePlayers, int currCol, int currRow){
   
    int counter = 0; 
    for (int j = 0; j < 4; j++){
        if ((currRow-j < 0) || (currCol-j < 0) || (grid[currRow - j][currCol - j] != (gamePlayers -> activePlayer))){
            counter = 0; 
            break; 
        }
        counter++; 
    } 

    for (int i = 0; i < 4; i++){
        //checking out of bounce to the right ?
        if((currRow+i > 5) || (currCol + i > 6) || (grid[currRow + i][currCol + i] != (gamePlayers -> activePlayer))){ 
            break; 
        }
        counter++; 
    }
    return (counter >= 4); 


}




int diagonalLeft (char** grid, struct players *gamePlayers, int currCol, int currRow){

    int counter = 0; 
    for (int i = 0; i < 4; i++){
        if((currRow-i < 0) || (currCol + i > 6) || (grid[currRow - i][currCol + i] != (gamePlayers -> activePlayer))){
            counter = 0; 
            break; 
        }
        counter++; 
    }

     for (int j = 0; j < 4; j++){

        if ((currRow+j > 5) || (currCol-j < 0) || (grid[currRow + j][currCol - j] != (gamePlayers -> activePlayer))){
            break; 
        }
        counter++; 
    } 
    return (counter >= 4); 
}


int horizontal (char** grid, struct players *gamePlayers, int currCol, int currRow){

    int counter = 0; 
   
    for (int i = 0; i < COLUMNS; i++){
       if (grid[currRow][i] != (gamePlayers -> activePlayer)){
        counter = 0;
        continue;     
       } 
       counter++;
       if (counter == 4) {
        break;
       }
    }
     
    return (counter == 4); 

}


int vertical (char** grid, struct players *gamePlayers, int currCol, int currRow){
    int counter = 0;
    if(currRow < 3){
        int i = 0; 
        while(i<4){
            if ((currRow+i > 5) || grid[currRow+i][currCol] != (gamePlayers -> activePlayer)){
                break; 
            }
            i++; 
            counter++;
        }
    }
    return counter >= 4; 
}





void winCase (char** grid, struct players *gamePlayers, int currCol, int currRow){

    if (diagonalLeft(grid, gamePlayers, currCol, currRow) || diagonalRight(grid, gamePlayers, currCol, currRow) ||
        vertical (grid, gamePlayers, currCol, currRow) || horizontal (grid, gamePlayers, currCol, currRow)){
         gamePlayers -> Winner = gamePlayers -> activePlayer;
         printf(" PLAYER %c WINS! \n", gamePlayers -> Winner); 
         
    } 
}


// Updates the grid everytime the player eneter's their input 
void updateGrid(int num, char** grid, struct players *gamePlayers){
    num = num - 1; 
    //error handling------
    for(int i = ROWS-1; i >= 0; i--){
        if (grid[i][num] == '_'){
            grid[i][num] = gamePlayers -> activePlayer; 
            winCase(grid, gamePlayers, num, i);
            break; 
        }
        continue; 
    }


}


//Prints the updated grid 
void printArr(char** grid){
    
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLUMNS; j++){
        printf("|%c", grid[i][j]); 
        }
          printf("| \n"); 
    }
}

//Updates the player everytime after the first player inputs a number 
void changePlayers(struct players *gamePlayers){
    if(gamePlayers -> activePlayer == PLAYER1){
        gamePlayers -> activePlayer = PLAYER2; 
    } else {
        gamePlayers -> activePlayer = PLAYER1;
    }
    printf("game player %c \n", gamePlayers -> activePlayer); 
}
   
   

void playGame (){

    char** grid = createArr();
    printArr(grid);  
    int input; 
    struct players *gamePlayers = malloc(sizeof(struct players)); 
    gamePlayers -> player1 = 'X'; 
    gamePlayers -> player2 = 'O'; 
    gamePlayers -> activePlayer = PLAYER1; 
    gamePlayers -> Winner = '_';

    for (int i = 0; i < 42; i++){
        printf("Input number: \n");
        scanf("%d", &input);
        //error handling------
        updateGrid(input, grid, gamePlayers); 
        printArr(grid); 
        changePlayers(gamePlayers);
        if( gamePlayers -> Winner != '_'){
            for (int i = 0; i < ROWS; i++){
                free(grid[i]);
            }
            free(grid);
            free(gamePlayers);
            return; 
        } 
    }
    //draw posibility-------

}





int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 3;
    }

    // Main game loop
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Add your game rendering logic here

        SDL_RenderPresent(renderer);  // Render to the screen
    }

    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
