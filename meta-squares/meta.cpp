/** An agent for the game, Meta Squares
 * @author Saeed Nejati
 */
#include <stdio.h>
#include <sys/time.h>
#include "meta.h"
#include "util.h"


int Agent::search(state &s, int depth, int alpha, int beta, int sMax, int sMin, int curPlayer, int &X, int &Y)
{
	if ( sMax > 150 && sMax - sMin >= 15 ) return INF;
	if ( sMin > 150 && sMin - sMax >= 15 ) return -INF;

	if ( depth == 0 ) return (sMax - sMin);

	int opponent = (curPlayer == BLACK) ? WHITE : BLACK;

	int i, j, x, y, a, b;
	int v, tmp;

	if ( curPlayer == player )
	{
		int cut = 0;
		for( i=0; i<SIZE; i++ )
			for( j=0; j<SIZE; j++ )
				if ( s.map[i][j] == EMPTY ) { x = i; y = j; }

		for( i=0; i<SIZE && !cut; i++ )
			for( j=0; j<SIZE && !cut; j++ )
			{
				if ( s.map[i][j] != EMPTY ) continue;

				s.map[i][j] = curPlayer;
				tmp = score(s, i, j);
				v = search(s, depth-1, alpha, beta, tmp+sMax, sMin, opponent, a, b);
				s.map[i][j] = EMPTY;
				if ( v > alpha )
				{
					alpha = v;
					x = i;
					y = j;
				}
				
				if ( beta <= alpha ) cut = 1;
			}
		X = x;
		Y = y;
		return alpha;
	}
	else
	{
		for( i=0; i<SIZE; i++ )
			for( j=0; j<SIZE; j++ )
			{
				if ( s.map[i][j] != EMPTY ) continue;

				s.map[i][j] = curPlayer;
				tmp = score(s, i, j);
				v = search(s, depth-1, alpha, beta, sMax, tmp+sMin, opponent, a, b);
				s.map[i][j] = EMPTY;
				if ( v < beta ) beta = v;
				
				if ( beta <= alpha ) return beta;
			}
		return beta;
	}

}

int Agent::move(state& board, int &X, int &Y)
{
	gettimeofday(&start, NULL);
	int sb = score(board, BLACK);
	int sw = score(board, WHITE);

	int value = search(board, thinkingDepth, -INF, INF, sb, sw, player, X, Y);
	gettimeofday(&end, NULL);

	return value;
}

long long Agent::getThinkingTime()
{
	return (((long long)end.tv_sec*1000000+end.tv_usec)-((long long)start.tv_sec*1000000+start.tv_usec)) / 1000;
}
