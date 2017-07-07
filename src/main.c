
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// define for extra printing during debugging
//#define DEBUG

#define GAME_ROWS 10
#define GAME_COLS 10
#define GAME_NUM_MINES 20

void rowSpacer( int numColumns );
void colHeader( int numColumns );
void tablePhil( int rowId, int numCol );


char gameMapChar[GAME_ROWS][GAME_COLS] = {0};
char gameMapCharKey[GAME_ROWS][GAME_COLS] = {0};


int main( int argc, char* argv[])
{
	system("clear");
	int mineMap[GAME_NUM_MINES][2] = {0};
	int gameMap[GAME_ROWS][GAME_COLS] = {0};
	int minesFound = 0;
	bool matchInArray = true;
	srand( time(NULL) );

	// prep game
	// future improvement: make game params 1) matix size & 2) number of mines user defined.

	// generate 20 random, individual mine loctions.
	int mineGeneratorCounter = 0;
	while( mineGeneratorCounter < GAME_NUM_MINES )
	{
		int rowRand = (int)rand() % GAME_ROWS;
		int colRand = (int)rand() % GAME_COLS;
		if( gameMap[rowRand][colRand] == 0 )
		{
			mineMap[mineGeneratorCounter][0] = rowRand;
			mineMap[mineGeneratorCounter][1] = colRand;
			gameMap[rowRand][colRand] = 255;
			mineGeneratorCounter++;
		}
	}

	// populate mine-adjacent squares with +1 for each adjacent mine.
	// checking done here to protect wirting outside bounds, corrupting data.
	for(int index = 0; index < GAME_NUM_MINES; index++ )
	{
		int mineRow = mineMap[index][0];
		int mineCol = mineMap[index][1];
		bool okRight = false;
		bool okLeft = false;
		bool okTop = false;
		bool okBottom = false;

		// check if adj rows are on edge
		if( (mineRow > 0) & (mineRow < (GAME_ROWS-1)) )
		{
			okTop = true;
			okBottom = true;
		}
		else
		{
			if(mineRow > 0) { okTop = true; okBottom = false; }
			else { okTop = false; okBottom = true; }
		}

		// check if adj cols are on edge
		if( (mineCol > 0) & (mineCol < (GAME_COLS-1)) )
		{
			okRight = true;
			okLeft = true;
		}
		else
		{
			if(mineCol > 0) { okRight = false; okLeft = true; }
			else { okRight = true; okLeft = false; }
		}

		// increment adjacent cells as necessary (i.e. dont incement if outside array bounds!)
		if( okTop ) { gameMap[mineRow-1][mineCol]++; }
		if( okBottom ) { gameMap[mineRow+1][mineCol]++; }
		if( okLeft ) { gameMap[mineRow][mineCol-1]++; }
		if( okRight ) { gameMap[mineRow][mineCol+1]++; }

		if( okTop & okLeft ) { gameMap[mineRow-1][mineCol-1]++; }
		if( okTop & okRight ) { gameMap[mineRow-1][mineCol+1]++; }
		if( okBottom & okLeft ) { gameMap[mineRow+1][mineCol-1]++; }
		if( okBottom * okRight ) { gameMap[mineRow+1][mineCol+1]++; }

	}

	// create game map key
	for(int index = 0; index < GAME_ROWS; index++ )
	{
		for( int index2 = 0; index2 < GAME_COLS; index2++ )
		{
			gameMapChar[index][index2] = '.';
			if( gameMap[index][index2] > 10 ) // 10 ok b/c no square can have >8 mines
				gameMapCharKey[index][index2] = 'M';
			else
				gameMapCharKey[index][index2] = gameMap[index][index2] + '0';
		}
	}

	// start playing the game
	printf("\nLet's start playing!\n");
	bool gameComplete = false;
	char c;
	int rowInput, colInput;
	bool enterKeyPressed = false;
	bool badMatrixSelection = false;
	bool mineID = false;
	bool cheatCodeRcvd = false;
	int countEntries = 0; // used to count input char position
	while( !gameComplete )
	{
		#ifdef DEBUG
		for(int index = 0; index < GAME_ROWS; index++ )
		{
			for( int index2 = 0; index2 < GAME_COLS; index2++ )
			{
				printf("%c,", gameMapCharKey[index][index2]);
			}
			printf("\n");
		}
		for( int index = 0; index < GAME_NUM_MINES; index++ )
		{
			printf("%d,%d\n", mineMap[index][0], mineMap[index][1]);
		}
		printf("\n");
		#endif
		
		// print game board matrix
		rowSpacer(GAME_COLS);
		colHeader(GAME_COLS);
		for( int indexTableRows = 0; indexTableRows < GAME_ROWS; indexTableRows++ )
		{
			rowSpacer(GAME_COLS);
			tablePhil(indexTableRows, GAME_COLS);
		}
		rowSpacer(GAME_COLS);

		//check for win (all squares of gameMapChar are turned over and haven't lost yet)
		bool mapComplete = true;
		for(int index = 0; index < 8; index++ )
		{
			for( int index2 = 0; index2 < 6; index2++ )
			{
				if( gameMapChar[index][index2] == gameMapCharKey[index][index2] )
				{
					mapComplete &= true;
				}
				else
				{
					mapComplete &= false;
				}
			}
		}
		// if map is complete, game over, user won, exit program.
		if( mapComplete )
		{
			if( true == cheatCodeRcvd )
			{
				printf("\nAweseome. You earned that one.\n");
			}
			else
			{
				printf("\nYou did it! you won the game!\n");
			}
			return 0;
		}

		// prep vars for processing user input
		badMatrixSelection = false;
		mineID = false;
		cheatCodeRcvd = false;
		countEntries = 0; // used to count input char position
		enterKeyPressed = false;

		// get user selection for game board element to turn.
		// collect all input characters before [ENTER]-key passed
		printf("Enter matrix address and press return to reveal: ");
		while( countEntries == 0)
		{
			c = getchar();
			if( (c > 0x40) & (c < (0x41+GAME_ROWS)) )
			{
				countEntries++;
				rowInput = (int)(c - 0x41);
			}
			else if( c == 'M')
			{
				mineID = true;
			}
			else if((c == 'Z') & (true == mineID))
			{
				countEntries++;
				cheatCodeRcvd = true;
			}
			else if( (c == 'q') & ( false == mineID) ) // special case to all user to quit application
			{
				printf("\nProgram Terminated.\n");
				return 0;
			}
			else if( c == 0x0A )
			{
				enterKeyPressed = true;
				badMatrixSelection = true;
			}
			else
			{
				countEntries++;
				badMatrixSelection = true;
			}
		}
		// if first inputs (row selection or mine check plus row id) passed, try for second
		if( !badMatrixSelection )
		{
			c = getchar();
			if( (c >= 0x30) & (c <= (0x30 + GAME_COLS - 1)))
			{
				colInput = (int)(c - 0x30);
			}
			else if( c == 0x0A)
			{
				enterKeyPressed = true;
				badMatrixSelection = true;
			}
			else
			{
				badMatrixSelection = true;
			}
		}
		// wait for enter key; ignore all other new inputs
		// enter key press implicit in any user input as it is required to send data
		while( false == enterKeyPressed )
		{
			if( 0x0A == getchar() )
			{
				enterKeyPressed = true;
			}
		}

		// Process user inputs
		system("clear"); // clear old information, prep terminal for new printout
		if( cheatCodeRcvd )
			{
				for(int index = 0; index < GAME_ROWS; index++ )
				{
					for( int index2 = 0; index2 < GAME_COLS; index2++ )
					{
						gameMapChar[index][index2] = gameMapCharKey[index][index2];
					}
				}
			}
		else if( true == badMatrixSelection )
		{
			printf("\nMatrix element not valid, please choose from available options in table.\n");
		}
		else if( true == mineID )
		{
			printf("Card: %c%c\n", rowInput+0x41, colInput+0x30);
			if(gameMapChar[rowInput][colInput] == '.')
			{
				if( gameMapCharKey[rowInput][colInput] == 'M' )
				{
					minesFound++;
					gameMapChar[rowInput][colInput] = 'M';
					printf("Congrats! You've identified a mine!\n");
				}
				else
				{
					printf("\nYou IDIOT! That wasn't a mine. Better luck next time.\n");
					return 0;
				}
			}
			else
			{
				printf("You already turned over that box! Try Again.\n");
			}
		}
		else
		{
			printf("Card: %c%c\n", rowInput+0x41, colInput+0x30);
			if(gameMapCharKey[rowInput][colInput] == 'M')
			{
				printf("\nYou tripped a land mine and died! Better luck next time!\n");
				return 0;
			}
			else if(gameMapChar[rowInput][colInput] == '.')
			{
				printf("Good selection!\n");
				gameMapChar[rowInput][colInput] = gameMapCharKey[rowInput][colInput];
			}
			else
			{
				printf("Square already selected, try again please...\n");
			}
		}
	}
	return(0);
}



void rowSpacer( int numColumns )
{
	printf("|---");
	for( int index = 0; index < numColumns; index++)
	{
		printf("|---");
	}
	printf("|\n");
}

void colHeader( int numColumns )
{
	printf("|   ");
	for( int index = 0; index < numColumns; index++)
	{
		printf("| %c ", (index+0x30));
	}
	printf("|\n");
}

void tablePhil( int rowId, int numCol )
{
	printf("| %c ", (rowId + 0x41) );
	for( int index = 0; index < numCol; index++ )
	{
		printf("| %c ", gameMapChar[rowId][index]);
		if( index >= GAME_COLS) { index =  numCol;} // don't write outside of array size
	}
	printf("|\n");

}



