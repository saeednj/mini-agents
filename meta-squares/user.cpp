#include "user.h"
#include <stdio.h>

int User::move(state& board, int &X, int &Y)
{
	int r, c;
	for(;;)
	{
		printf("\nYour move: ");
		scanf("%d", &r);
		c = r % 10;
		r /= 10;
		if ( r>8 || c>8 || board.map[r-1][c-1] != EMPTY ) puts("Invalid Move!");
		else break;
	}
	X = r - 1;
	Y = c - 1;
	return 0;
}

long long User::getThinkingTime()
{
	return -1;
}
