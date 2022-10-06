// "Assignment Three"
//////////////////////////////////
// Program Name : CSE
// Author: Daniel Pintur & Tom Lloyd
// Student # : 8713390
// Student Email : Dpintur3390@conestogac.on.ca
// Date: Sept 27th, 2022
// Class: PRDG 71985k
//////////////////////////////////


/*
[ D	] 1. Constantly animate a snake on the terminal within the game board
[ D	] 2. At the start the body of the snake should be of size 1
[ D	] 3. Use a 1D global integer array to populate the snake body coordinates as it grows
[ D	] 4. The initial position of the snake can be any valid cell on the board picked at random
[ D	] 5. Set a user defined key to grow the body of the snake
[ D	] 6. Specify 4 keys for directing the snake on the terminal
[ D	] 7. The board should be 30 x 30 in size, and the edges should be clearly marked
[ D] 8. The edge cells are not valid cells where the snake body should be
[ D	] 9. If the snake body goes through an edge, it should wrap around the game board boundaries
[ D	] 10. Specify a key, that when pressed quits the program
[ D	] 11. Use a 2D global char array to maintain what you will draw on the board each iteration.
*/

//Function library
#include <stdio.h> 
#include <unistd.h>
#include <windows.h>
#include <conio.h> 
#include <stdlib.h>
#include <time.h> 

#define h 25 //array height
#define w 50 //array width 
#define DelayConstant 50000 //speed of system clear

// variables for the nested for loop
int i,j;
//2D array for the board boundries
char board[h][w];
// 1D global integers for the snake body
int snake_x[h*w]; int snake_y[h*w]; int snake_size = 1;
int temp_x[h*w];
int temp_y[h*w];
//2D array for the fruit
int fruit[h][w];
int num_size =1;

//coordinate points for the position of the snake
int x,y; 
// conio ints to setup _kbhit()
int input;
int key;


// hides the cursor during each system clear
void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Setup char array to hold borders and empty spaces.
void init_board()
{
    srand(time(NULL));   // Set seed of random with time of computer.
    snake_x[0]=rand() % h; snake_y[0]=rand() % w; key='w';
    for (int i=0; i<h;i++)
	{
        for (int j=0;j<w;j++)
		{
            if (i == 0 || i == (h-1) || j== 0 || j == (w-1))
			{
                board[i][j] = '#';
            } 
			else
			{
                board[i][j] = ' ';
            }
        }
    }
}
// After every render, redraw blank board to remove the older snake parts from the moving array that shouldnt be there anymore
void refresh_board()
{
    for (int i=0; i<h;i++)
	{
        for (int j=0;j<w;j++)
		{
            if (i == 0 || i == (h-1) || j== 0 || j == (w-1))
			{
                board[i][j] = '#';
            } 
			else
			{
                board[i][j] = ' ';
            }
        }
    }
}


//For every snake part, determined by snake size, will set the board state to '*' at the given snake coordinate.
void draw_snake()
{
    refresh_board();
    // loop through snake arrays to add snake to board.
    for (i=0;i<snake_size;i++)
	{
        if (i ==0)
		{
            board[snake_y[i]][snake_x[i]] = '@';
        }
		 else
		{
            board[snake_y[i]][snake_x[i]] = '$';
        }
    }
}


// Add snake head to tail of snake.
// used if (dir) to 
// Function to 
void grow_snake(int dir)
{
    snake_size++;
    int index = snake_size -1;
    if (dir == 0)
	{
        // moving left so add snake to the right.
        // Set new snake part x-coordinate to the previous + 1.
        snake_x[index] = snake_x[index-1] + 1;
        snake_y[index] = snake_y[index-1];
    }
	 else if (dir == 1)
	{
        // moving right so add snake to the left.
        snake_x[index] = snake_x[index-1] - 1;
        snake_y[index] = snake_y[index-1];
    } 
	else if (dir == 2)
	{
        // moving up so add snake to below.
        snake_x[index] = snake_x[index-1];
        snake_y[index] = snake_y[index-1] + 1;
    } 
	else
	{
        // moving down so add snake to up.
        snake_x[index] = snake_x[index-1];
        snake_y[index] = snake_y[index-1] -1;
    }
}

// Perform copy of snake arrays into temp arrays.
void copy_temp()
{
    for (i=0;i<snake_size;i++)
	{
        temp_x[i] = snake_x[i];
        temp_y[i] = snake_y[i];
    }
}

/**
 * Snake moving up
 */
void move_up()
{
    // move head of snake up

    // for all snake parts determine where the snake part needs to go in order to move up.
    copy_temp();
    for (i=1; i< snake_size; i++)
	{
        snake_x[i] = temp_x[i - 1];
        snake_y[i] = temp_y[i - 1];
        // make sure the snake parts pass through to other side of board.
        if (snake_y[i]==0)
		{
            snake_y[i]=h-2;
        }
    }
    snake_y[0]--;

    // make sure the head of snake passes through to other side of board.
    if(snake_y[0]==0)
    {
        snake_y[0]=h-2;
    }
}
// similar logic as void move_up but specific for each direction and desired location of the added body part.
void move_down()
{
    copy_temp();
    for (i=1; i< snake_size; i++)
	{
        snake_x[i] = temp_x[i - 1];
        snake_y[i] = temp_y[i - 1];

        if (snake_y[i]==h-1)
		{
            snake_y[i]=1;
        }
    }
    snake_y[0]++;

    if(snake_y[0]==h-1)
    {
        snake_y[0]=1;
    }
}

void move_left()
{
    copy_temp();
    for (i=1; i< snake_size; i++)
	{
        snake_x[i] = temp_x[i - 1];
        snake_y[i] = temp_y[i - 1];
        if (snake_x[i]==1)
		{
            snake_x[i]=w-2;
        }
    }
    snake_x[0]--;

    if(snake_x[0]==1)
    {
        snake_x[0]=w-2;
    }
}

void move_right()
{
    copy_temp();
    for (i=1; i< snake_size; i++)
	{
        snake_x[i] = temp_x[i - 1];
        snake_y[i] = temp_y[i - 1];
        if (snake_x[i] == w - 1)
		{
            snake_x[i] = 1;
        }
    }
    snake_x[0]++;

    if(snake_x[0]==w-1)
    {
        snake_x[0]=1;
    }
}

int keyInput()
{
	if(_kbhit()) 
		return _getch();
	else
		return -1;
}
/*
 *
 * return 0 - left, 1 - right, 2- up, 3-down
 */
int getUserInput(int last_dir)
{
	input = keyInput();
	//code to not allow user to go 180 degrees in the other direction of the moving snake.
	// if a direction's assci value is within 5 values of one another it won't register the key press
    if (((input==100||input==97||input==119||input==115) &&(abs(key-input)>5)))
    {   
		key=input;
	}
    if(input == 32) //spacebar
    {
        grow_snake(last_dir);
    }
    if (input == 27) //esc
    {
        printf("\n\n		SEE YA LATER\n\n");
        exit(0);
    }
 	if(key == 100) //d
	{
       move_right();
        return 1;
	}
	if(key == 97) //a
	{
        move_left();
        return 0;
	}
	if(key == 119)//w
	{
        move_up();
        return 2;
	}
	if(key == 115)//s
	{
        move_down();
        return 3;
	}
}

int self_collision()
{
	//i =1 for the snake_size.
	//if snake head [0] touchs any value of the x or y 1D snake array then return 1. 
	for(i=1;i<snake_size;i++)
	{
		if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
		{
			return 1;
		}
	
	}
	return 0;
}


// Main function that starts execution of the program
int main (void)
{
	hidecursor();
	refresh_board();
    init_board();
    // Init direction of snake.
    int dir = 0;
	// while loop for repetition of the game
	while (1)
	{
	
		dir = getUserInput(dir);
        draw_snake(dir);

        // draw the board state to the console.
	    for (i=0; i<h;i++)
		{
            for (j=0;j<w;j++)
			{
                printf("%c", board[i][j]);
            }
            // for every row print a new line.
            printf("\n");
        }
        if(self_collision()==1)
        {
        	printf("YOU SUCK GAME OVER");
        	break;
		}
        //helped to understand the direction of the snake to program the move options
        printf("\n");
        printf("WSAD to control the snake, spacebar to add body parts\n");
        printf("direction: %d\n", dir);
        printf("snake size: %d\n", snake_size);
		// usleep makes the program sleep for a specified time - pulled from the defined DELAY_CONSTANT.
		// system("cls") clears the console after each char is moved
        usleep(DelayConstant);
        system("cls");
	}
	
}