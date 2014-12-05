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

	Player *player[2];

	player[0] = new Agent(BLACK, 4);
	player[1] = new User(WHITE);

	int pid = 0;

	for(;;)
	{
		int x, y;
		long long thinkingTime; 

		player[pid]->move(board, x, y);
		thinkingTime = player[pid]->getThinkingTime();

		board.map[x][y] = player[pid]->getPlayer();
		show(board);
		printf("%s's move: %02d\n", toString(player[pid]->getPlayer()), (x+1)*10+y+1);
		if ( thinkingTime != -1 )
			printf("thinking time = %lld ms\n", thinkingTime);

		pid = 1 - pid;

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
