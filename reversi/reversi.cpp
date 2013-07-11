/**
 * An AI agent for the game Reversi (othello)
 * @author Saeed Nejati
 */
#include <stdio.h>
#include <sys/time.h>

#define SIZE 8
#define X 1
#define O -1
#define INF 2000000000
#define THINKING_DEPTH 6

#define inRange(x, y) ((x)>=0 && (x)<SIZE && (y)>=0 && (y)<SIZE)

int map[SIZE][SIZE];
int stateCount;

/** Prints a colorful board */
void print()
{
	int i, j;
	puts("");
	printf("     A   B   C   D   E   F   G   H  \n");
	printf("   +---+---+---+---+---+---+---+---+\n");
	for( i=0; i<SIZE; i++ )
	{
		printf(" %d ", i+1);
		for( j=0; j<SIZE; j++ )
		{
			if ( map[i][j] == X )
				printf("| \033[31mX\033[0m ");
			else if ( map[i][j] == O )
				printf("| \033[34mO\033[0m ");
			else
				printf("|   ");
		}
		printf("|\n   +---+---+---+---+---+---+---+---+\n");
	}
	puts("");
}

/** checks if a player can put his peg on cell (x, y) */
int valid(int x, int y, int player)
{
	if ( !inRange(x, y) ) return 0;
	if ( map[x][y] ) return 0;
	for( int dx=-1; dx<=1; dx++ )
		for( int dy=-1; dy<=1; dy++ )
		{
			int A = 0, B = 0;
			for( int i=x+dx,j=y+dy; inRange(i, j); i+=dx,j+=dy )
				if ( map[i][j] == -player ) B++;
				else if ( map[i][j] == player ) { A = 1; break; }
				else if ( map[i][j] == 0 ) break;
			if ( A == 1 && B > 0 ) return 1;
		}
	return 0;
}

/** checks if a player has any valid move on the board */
int hasValidMove(int player)
{
	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
		{
			if ( map[i][j] ) continue;
			if ( valid(i, j, player) ) return 1;
		}
	return 0;
}

/** Performs a move for player (t) by putting
 * his peg on cell (x, y) and flipping necessary pegs */
void move(int x, int y, int t)
{
	int i, j, I, J, dx, dy;
	int A, B;
	map[x][y] = t;
	for( dx=-1; dx<=1; dx++ )
		for( dy=-1; dy<=1; dy++ )
		{
			A = B = 0;
			for( i=x+dx,j=y+dy; inRange(i, j); i+=dx,j+=dy )
				if ( map[i][j] == -t ) B++;
				else if ( map[i][j] == t ) { A = 1; break; }
				else if ( map[i][j] == 0 ) break;
			if ( A == 1 && B > 0 )
				for( I=x+dx,J=y+dy; I!=i || J!=j; I+=dx,J+=dy )
					map[I][J] = t;
		}
}

/** Checks if any player has won the game
 * @return 1   if X is winner
 *         -1  if O is winner
 *         0   if Draw
 *         INF if not finished
 */
int winner()
{
	int i, j;
	int x, o, e;
	x = o = e = 0;
	for( i=0; i<SIZE; i++ )
		for( j=0; j<SIZE; j++ )
			if ( map[i][j] == 1 ) x++;
			else if ( map[i][j] == -1 ) o++;
			else e++;
	if ( e == 0 ) return (x>o ? X : x<o ? O : 0);
	if ( o == 0 && x > 0 ) return X;
	if ( o > 0 && x == 0 ) return O;
	if ( !hasValidMove(X) && !hasValidMove(O) ) return (x>o ? X : x<o ? O : 0);
	return INF;
}

/** Calculates the score of a board
 *  (leaf node) */
int score()
{
	int weight[SIZE][SIZE];
	int x = 0, o = 0, e = 0;
	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
		{
			if ( i == 0 || i == SIZE-1 || j == 0 || j == SIZE-1 ) weight[i][j] = 2;
			else if ( i == 1 || i == SIZE-2 || j == 1 || j == SIZE-2 ) weight[i][j] = -1;
			else weight[i][j] = 1;
			if ( map[i][j] == X ) x++;
			else if ( map[i][j] == O ) o++;
			else e++;
		}
	weight[1][1] = weight[1][0] = weight[0][1] = (map[0][0] == 1) ? 2 : -1;
	weight[1][SIZE-2] = weight[1][SIZE-1] = weight[0][SIZE-2] = (map[0][SIZE-1] == 1) ? 2 : -1;
	weight[SIZE-2][1] = weight[SIZE-1][1] = weight[SIZE-2][0] = (map[SIZE-1][0] == 1) ? 2 : -1;
	weight[SIZE-2][SIZE-2] = weight[SIZE-1][SIZE-2] = weight[SIZE-2][SIZE-1] = (map[SIZE-1][SIZE-1] == 1) ? 2 : -1;

	weight[0][0] = weight[0][SIZE-1] = weight[SIZE-1][0] = weight[SIZE-1][SIZE-1] = 5;

	int s = 0;
	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
			s += weight[i][j] * map[i][j];
	return s;
}

/** The main alpha-beta search function */
int value(int player, int* x, int* y, int depth, int alpha, int beta)
{
	int w = winner();
	if ( w != INF ) return w*INF;

	if ( depth == 0 ) return score();

	int dummyX, dummyY;
	int xx = -1, yy = -1;
	int tmp[SIZE][SIZE];
	int cut = 0;

	stateCount++;

	for( int i=0; i<SIZE && !cut; i++ )
		for( int j=0; j<SIZE && !cut; j++ )
		{
			if ( map[i][j] ) continue;
			if ( !valid(i, j, player) ) continue;
			if ( player == X && xx == -1 ) { xx = i; yy = j; }

			for( int p=0; p<SIZE; p++ )
				for( int q=0; q<SIZE; q++ )
					tmp[p][q] = map[p][q];
			
			move(i, j, player);
			int v = value(-player, &dummyX, &dummyY, depth-1, alpha, beta);
			
			for( int p=0; p<SIZE; p++ )
				for( int q=0; q<SIZE; q++ )
					map[p][q] = tmp[p][q];

			if ( player == X )
			{
				if ( v > alpha )
				{
					alpha = v;
					xx = i;
					yy = j;
				}
			}
			else
			{
				if ( v < beta ) beta = v;
			}

			if ( beta <= alpha ) cut = 1;
		}

	if ( player == X )
	{
		*x = xx;
		*y = yy;
		return alpha;
	}
	return beta;
}

/** Processes a computer move */
void move_X()
{
	int x, y, v;
	long long start, end;
	struct timeval tv;

	stateCount = 0;

	gettimeofday(&tv, NULL);
	start = (long long)tv.tv_sec * 1000000 + tv.tv_usec;

	v = value(X, &x, &y, THINKING_DEPTH, -INF, INF);

	gettimeofday(&tv, NULL);
	end = (long long)tv.tv_sec * 1000000 + tv.tv_usec;
	end -= start;

	printf("%d states has been processed\n", stateCount);
	printf("Thinking time: %lld:%lld.%lld\n", end/1000000, (end%1000000)/1000, end%1000);
	printf("X's move is (%c%d) with value: %d\n", y+'A', x+1, v);

	move(x, y, X);
}

/** Processes a user move */
void move_O()
{
	int i, j;
	char s[5];
	for( ; ; )
	{
		scanf("%s", s);
		j = (s[0]>='A' && s[0]<='H')?s[0]-'A':(s[0]>='a' && s[0]<='h')?s[0]-'a':-1;
		i = (s[1]>='1' && s[1]<='8')?s[1]-'1':-1;
		if ( !valid(i, j, O) )
			puts("Invalid Move");
		else break;
	}
	move(i, j, O);
}

int main()
{
	int w, turn;
	char c;


	map[SIZE/2-1][SIZE/2-1] = map[SIZE/2][SIZE/2] = O;
	map[SIZE/2-1][SIZE/2] = map[SIZE/2][SIZE/2-1] = X;

	printf("Computer is X. Who will start the game (x/o)? ");
	scanf("%c", &c);
	turn = (c == 'x') ? 1 : 0;
	print();
	for( ; ; )
	{
		if ( !hasValidMove(2*turn-1) )
		{
			printf("%c has no valid moves, %c should play.\n", (turn?'X':'O'), (turn?'O':'X'));
			turn = 1 - turn;
		}
		if ( turn ) move_X(); else move_O();
		turn = 1 - turn;
		print();
		w = winner();
		if ( w != INF ) break;
	}

	int x = 0, o = 0;
	for( int i=0; i<SIZE; i++ )
		for( int j=0; j<SIZE; j++ )
			if ( map[i][j] == X ) x++;
			else if ( map[i][j] == O ) o++;

	printf("X: %d, O: %d\n", x, o);
	if ( w == X ) puts("X won..");
	if ( w == O ) puts("O won..");
	if ( w == 0 ) puts("Draw..");
	return 0;
}
