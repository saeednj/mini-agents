#ifndef _DEFS_H_
#define _DEFS_H_

#define SIZE 8

#define BLACK 1
#define WHITE -1
#define EMPTY 0

#define DRAW 0

#define INF 2000000000

#define inRange(x, y) ((x)>=0 && (x)<SIZE && (y)>=0 && (y)<SIZE)
#define toString(player) ((player) == BLACK ? "BLACK" : "WHITE")

struct state
{
	int map[SIZE][SIZE];
};


#endif
