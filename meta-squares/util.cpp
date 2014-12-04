#include <stdio.h>
#include "util.h"

void show(state& s)
{
	int i, j;
	printf("\033[2J\033[1;1H");
	printf("     1   2   3   4   5   6   7   8  \n");
	printf("   +---+---+---+---+---+---+---+---+\n");
	for( i=0; i<SIZE; i++ )
	{
		printf(" %d ", i+1);
		for( j=0; j<SIZE; j++ )
		{
			if ( s.map[i][j] == BLACK )
				printf("| \033[31m@\033[0m ");
			else if ( s.map[i][j] == WHITE )
				printf("| \033[34mO\033[0m ");
			else
				printf("|   ");
		}
		printf("|\n   +---+---+---+---+---+---+---+---+\n");
	}
	puts("");
}

int score(state &s, int i, int j)
{
	int player = s.map[i][j];
	int x, y, r = 0;
	for( x=0; x<SIZE; x++ )
		for( y=0; y<SIZE; y++ )
		{
			if ( x == i && y == j ) continue;
			if ( s.map[x][y] != player ) continue;
			int dx = x - i;
			int dy = y - j;
			if ( inRange(x-dy, y+dx) && inRange(i-dy, j+dx) && 
					s.map[x-dy][y+dx] == player && s.map[i-dy][j+dx] == player )
			{
				if ( dx < 0 ) dx = -dx;
				if ( dy < 0 ) dy = -dy;
				r += (dx+dy+1)*(dx+dy+1);
			}
		}
	return r;
}

int score(state &s, int player)
{
	int i, j, x, y;
	int r = 0;
	for( i=0; i<SIZE; i++ )
		for( j=0; j<SIZE; j++ )
		{
			if ( s.map[i][j] != player ) continue;
			for( x=0; i+x<SIZE; x++ )
				for( y=1; j+y<SIZE; y++ )
				{
					int b = i+x+y < SIZE &&
						j+y-x >= 0 && j+y-x < SIZE &&
						i+y >= 0 && i+y < SIZE &&
						j-x >= 0 && j-x < SIZE;
					if ( b && s.map[i+x][j+y] == player && s.map[i+x+y][j+y-x] == player && s.map[i+y][j-x] == player )
						r += (x+y+1)*(x+y+1);
				}
		}
	return r;
}
