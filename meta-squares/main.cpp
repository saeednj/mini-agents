#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "meta.h"
#include "util.h"

int main()
{
	state board;
	struct timeval start, end;
	int cnt = 64;

	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
			board.map[i][j] = EMPTY;

	int sw = 0, sb = 0;
	char first[2];

	Agent black(BLACK, 4);
	Agent white(WHITE, 4);

	int player = BLACK;

	int x, y;

	for(;;)
	{
		if ( player == BLACK )
		{
			gettimeofday(&start, NULL);
			black.move(board, x, y);
			gettimeofday(&end, NULL);
		}
		else
		{
			gettimeofday(&start, NULL);
			white.move(board, x, y);
			gettimeofday(&end, NULL);
		}
		board.map[x][y] = player;
		show(board);
		printf("%s's move: %02d\n", ( player == BLACK ? "BLACK" : "WHITE"), (x+1)*10+y+1);
		printf(" thinking time = %lld ms\n", (((long long)end.tv_sec*1000000+end.tv_usec)-((long long)start.tv_sec*1000000+start.tv_usec))/1000);

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
