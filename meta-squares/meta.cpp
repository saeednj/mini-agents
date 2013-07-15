/** An agent for the game, Meta Squares
 * @author Saeed Nejati
 */
#include <stdio.h>
#include <sys/time.h>
#define BLACK 1
#define WHITE -1
#define DRAW 0
#define EMPTY 0

#define SIZE 8
#define INF 2000000000
#define THINKING_LEVEL 4

#define inRange(x, y) ((x)>=0 && (x)<SIZE && (y)>=0 && (y)<SIZE)

struct state
{
	int map[SIZE][SIZE];
};

/** Prints board with ascii chars */
void show(state& s)
{
	int i, j;
	puts("");
	printf("     1   2   3   4   5   6   7   8  \n");
	printf("   +---+---+---+---+---+---+---+---+\n");
	for( i=0; i<SIZE; i++ )
	{
		printf(" %d ", i+1);
		for( j=0; j<SIZE; j++ )
		{
			if ( s.map[i][j] == BLACK )
				printf("| @ ");
			else if ( s.map[i][j] == WHITE )
				printf("| O ");
			else
				printf("|   ");
		}
		printf("|\n   +---+---+---+---+---+---+---+---+\n");
	}
	puts("");
}

/** Calculates score difference by putting a peg in cell (i,j) */
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

/** Calculates the game score of 'player' in current state */
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

/** alpha-beta search routine
 *  @param s Current state
 *  @param depth Search tree depth
 *  @param alpha Lower bound of evaluation score
 *  @param beta Upper bound of evaluation score
 *  @param sMax Game score of AI player
 *  @param sMin Game score of Opponent player
 *  @param player current player
 *  @param X AI result row
 *  @param Y AI result column
 *  @param maxPlayer indicates the AI player
 *  @return Evaluation score of a move
 */
int search(state &s, int depth, int alpha, int beta, int sMax, int sMin, int player, int *X, int *Y, int maxPlayer)
{
	if ( sMax > 150 && sMax - sMin >= 15 ) return INF;
	if ( sMin > 150 && sMin - sMax >= 15 ) return -INF;

	if ( depth == 0 ) return (sMax - sMin);

	int i, j, x, y, a, b;
	int v, tmp;

	if ( player == maxPlayer )
	{
		int cut = 0;
		for( i=0; i<SIZE; i++ )
			for( j=0; j<SIZE; j++ )
				if ( s.map[i][j] == EMPTY ) { x = i; y = j; }

		for( i=0; i<SIZE && !cut; i++ )
			for( j=0; j<SIZE && !cut; j++ )
			{
				if ( s.map[i][j] != EMPTY ) continue;

				s.map[i][j] = player;
				tmp = score(s, i, j);
				v = search(s, depth-1, alpha, beta, tmp+sMax, sMin, -player, &a, &b, maxPlayer);
				s.map[i][j] = EMPTY;
				if ( v > alpha )
				{
					alpha = v;
					x = i;
					y = j;
				}
				
				if ( beta <= alpha ) cut = 1;
			}
		*X = x;
		*Y = y;
		return alpha;
	}
	else
	{
		for( i=0; i<SIZE; i++ )
			for( j=0; j<SIZE; j++ )
			{
				if ( s.map[i][j] != EMPTY ) continue;

				s.map[i][j] = player;
				tmp = score(s, i, j);
				v = search(s, depth-1, alpha, beta, sMax, tmp+sMin, -player, &a, &b, maxPlayer);
				s.map[i][j] = EMPTY;
				if ( v < beta ) beta = v;
				
				if ( beta <= alpha ) return beta;
			}
		return beta;
	}

}

/** Processes an AI move */
int agentMove(state& board, int depth, int player, int *X, int *Y, int maxPlayer)
{
	int sb = score(board, BLACK);
	int sw = score(board, WHITE);

	return search(board, depth, -INF, INF, sb, sw, BLACK, X, Y, BLACK);
}

/** Handles a move by AI or User */
void move(state& board, int player)
{
	int r, c, x, y;

	if ( player == BLACK )
	{
		timeval start, end;
		gettimeofday(&start, NULL);
		int v = agentMove(board, THINKING_LEVEL, BLACK, &x, &y, BLACK);
		gettimeofday(&end, NULL);
		board.map[x][y] = BLACK;
		show(board);
		printf("Computer's move: %02d\n", (x+1)*10+y+1);
		printf(" thinking time = %lld ms\n", (((long long)end.tv_sec*1000000+end.tv_usec)-((long long)start.tv_sec*1000000+start.tv_usec))/1000);
	}
	else
	{
		for(;;)
		{
			printf("\nYour move: ");
			scanf("%d", &r);
			c = r % 10;
			r /= 10;
			if ( r>8 || c>8 || board.map[r-1][c-1] != EMPTY ) puts("Invalid Move!");
			else break;
		}
		board.map[r-1][c-1] = WHITE;
		show(board);
	}
}

int main()
{
	state board;
	int i, j;
	int cnt = 64;
	for( i=0; i<SIZE; i++ )
		for( j=0; j<SIZE; j++ )
			board.map[i][j] = EMPTY;

	int sw = 0, sb = 0;
	char first[2];

	printf("Computer is Black. Who moves first (b/w)? ");
	scanf("%s", first);
	if ( first[0] == 'b' || first[0] == 'B' )
	{
		move(board, BLACK);
		sb = score(board, BLACK);
		cnt--;
		printf(" white = %3d, black = %3d\n", sw, sb);
	}

	for(;;)
	{
		move(board, WHITE);
		sw = score(board, WHITE);
		printf(" white = %3d, black = %3d\n", sw, sb);
		if ( sw >= 150 && sw-sb >= 15 )
		{
			puts("WHITE WON");
			break;
		}
		cnt--;

		move(board, BLACK);
		sb = score(board, BLACK);
		printf(" white = %3d, black = %3d\n", sw, sb);
		if ( sb >= 150 && sb-sw >= 15 )
		{
			puts("BLACK WON");
			break;
		}
		cnt--;

		if ( cnt == 0 )
		{
			puts("DRAW");
			break;
		}
	}

	return 0;
}

