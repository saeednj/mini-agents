#include <stdio.h>
#include <unistd.h>
#include "meta.h"
#include "util.h"
#include "user.h"

int main()
{
	state board;
	int cnt = 64;

	/* Preparing a clear board */
	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
			board.map[i][j] = EMPTY;

	int sw = 0, sb = 0;
	char first[2];

	Agent comp(BLACK, 4);
	User human(WHITE);

	int player = BLACK;

	int x, y;

	for(;;)
	{
		long long thinkingTime; 
		if ( player == BLACK )
		{
			comp.move(board, x, y);
			thinkingTime = comp.getThinkingTime();
		}
		else
		{
			human.move(board, x, y);
		}
		board.map[x][y] = player;
		show(board);
		printf("%s's move: %02d\n", ( player == BLACK ? "BLACK" : "WHITE"), (x+1)*10+y+1);
		if ( player == BLACK )
			printf(" thinking time = %lld ms\n", thinkingTime);

		player = (player == BLACK) ? WHITE : BLACK;

		sb = score(board, BLACK);
		sw = score(board, WHITE);
		printf(" white = %3d, black = %3d\n", sw, sb);
		if ( sb >= 150 && sb-sw >= 15 )
		{
			puts("BLACK WON");
			break;
		}
		else if ( sw >= 150 && sw-sb >= 15 )
		{
			puts("WHITE WON");
			break;
		}

		cnt--;

		if ( cnt == 0 )
		{
			puts("DRAW");
			break;
		}
		sleep(2);
	}

	return 0;
}
