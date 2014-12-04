#ifndef _UTIL_H_
#define _UTIL_H_

#include "defs.h"

/** Prints board with ascii chars */
void show(state& s);

/** Calculates the game score of 'player' in current state */
int score(state &s, int player);

/** Calculates score difference by putting a peg in cell (i,j) */
int score(state &s, int i, int j);

#endif
