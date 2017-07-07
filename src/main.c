
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define GAME_ROWS 10
#define GAME_COLS 10
#define GAME_NUM_MINES 20

void rowSpacer( int numColumns );
void colHeader( int numColumns );
void tablePhil( int rowId, int numCol );


bool checkArrayForMatch_Mx2( int arrayToCheck[GAME_NUM_MINES][2])
{
	for( int index = 0; index < GAME_NUM_MINES; index++ )
	{
		int rowValToCheck = arrayToCheck[index][0];
		int colValToCheck = arrayToCheck[index][1];
		for( int indexCompare = 0; indexCompare < GAME_NUM_MINES; indexCompare++ )
		{
			if(index != indexCompare)
			{
				if( (rowValToCheck == arrayToCheck[indexCompare][0]) &
					(colValToCheck == arrayToCheck[indexCompare][1]) )
				{
					return(true); // there was a matching set, return true
				}
			}
		}
	}
	return(false);
}


char gameMapChar[GAME_ROWS][GAME_COLS] = {0};
char gameMapCharKey[GAME_ROWS][GAME_COLS] = {0};


int main( int argc, char* argv[])
{
	system("clear");
	int mineMap[GAME_NUM_MINES][2] = {0};
	int gameMap[GAME_ROWS][GAME_COLS] = {0};
	bool matchInArray = true;
	srand( time(NULL) );


	while( matchInArray )
	{
		for(int index = 0; index < GAME_NUM_MINES; index++ )
		{
			mineMap[index][0] = (int)rand() % GAME_ROWS;
			mineMap[index][1] = (int)rand() % GAME_COLS;
		}
		matchInArray = checkArrayForMatch_Mx2( mineMap );
	}

	for(int indexFillGameMap = 0; indexFillGameMap < GAME_NUM_MINES; indexFillGameMap++ )
	{
		gameMap[mineMap[indexFillGameMap][0]][mineMap[indexFillGameMap][1]] = 255;
	}
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

	
	printf("\nLet's start playing!\n");
	bool gameComplete = false;
	char c;
	int rowInput, colInput;
	while( !gameComplete )
	{

		// for(int index = 0; index < GAME_ROWS; index++ )
		// {
		// 	for( int index2 = 0; index2 < GAME_COLS; index2++ )
		// 	{
		// 		printf("%c,", gameMapCharKey[index][index2]);
		// 	}
		// 	printf("\n");
		// }
		// for( int index = 0; index < GAME_NUM_MINES; index++ )
		// {
		// 	printf("%d,%d\n", mineMap[index][0], mineMap[index][1]);
		// }
		// printf("\n");
		
		rowSpacer(GAME_COLS);
		colHeader(GAME_COLS);
		for( int indexTableRows = 0; indexTableRows < GAME_ROWS; indexTableRows++ )
		{
			rowSpacer(GAME_COLS);
			tablePhil(indexTableRows, GAME_COLS);
		}
		rowSpacer(GAME_COLS);

		//check for win
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
		if( mapComplete )
		{
			printf("\nYou did it! you won the game!\n");
			return 0;
		}
		printf("Enter matrix address and press return to reveal: ");
		bool transmisionComplete = false;
		bool badMatrixSelection = false;
		bool mineID = false;
		bool cheatCodeRcvd = false;
		int countEntries = 0;
		while( !transmisionComplete )
		{
			c = getchar();
			if( countEntries == 0)
			{
				if( (c >= 0x41) & (c <= (0x41+GAME_ROWS-1)))
				{
					rowInput = (int)(c - 0x41);
				}
				else if( c == 'M')
				{
					countEntries--;
					mineID = true;
				}
				else if(c == 'Z')
				{
					cheatCodeRcvd = true;
				}
				else
				{
					//rowInput = 0;
					badMatrixSelection = true;
				}

			}
			else if( countEntries == 1)
			{
				if( (c >= 0x30) & (c <= (0x30 + GAME_COLS - 1)))
				{
					colInput = (int)(c - 0x30);
				}
				else
				{
					//colInput = 0;
					badMatrixSelection = true;
				}
			}

			if( c == 0x0A )
			{
				transmisionComplete = true;
			}
			else if( c == 'q')
			{
				printf("\nProgram Terminated.\n");
				return 0;
			}
			countEntries++;
		}
		// printf("\n");
		system("clear");

		if(mineID)
		{
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
			else
			{
				gameMapChar[rowInput][colInput] = 'M';
				if( gameMapChar[rowInput][colInput] == gameMapCharKey[rowInput][colInput] )
				{
					printf("\nCongrats! You've identified a mine!\n");
				}
				else
				{
					printf("\nYou IDIOT! That wasn't a mine. Better luck next time.\n");
					return 0;
				}
			}
		}
		else if( badMatrixSelection )
		{
			printf("\nMatrix element not valid, please choose from available options in table.\n");
		}
		else
		{
			if(gameMapCharKey[rowInput][colInput] == 'M')
			{
				printf("\nYou tripped a land mine and died! Better luck next time!\n");
				return 0;
			}
			else if(gameMapChar[rowInput][colInput] == '.')
			{
				printf("\nGood selection!\n");
				gameMapChar[rowInput][colInput] = gameMapCharKey[rowInput][colInput];
			}
			else
			{
				printf("\nSquare already selected, try again please...\n");
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



