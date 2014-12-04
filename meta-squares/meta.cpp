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
				v = search(s, depth-1, alpha, beta, tmp+sMax, sMin, -curPlayer, a, b);
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
				v = search(s, depth-1, alpha, beta, sMax, tmp+sMin, -curPlayer, a, b);
				s.map[i][j] = EMPTY;
				if ( v < beta ) beta = v;
				
				if ( beta <= alpha ) return beta;
			}
		return beta;
	}

}

int Agent::move(state& board, int &X, int &Y)
{
	int sb = score(board, BLACK);
	int sw = score(board, WHITE);

	return search(board, thinkingDepth, -INF, INF, sb, sw, player, X, Y);
}

