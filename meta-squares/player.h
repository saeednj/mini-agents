#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defs.h"

class Player
{
	public:
		Player(int p = BLACK) : player(p) {}
		virtual ~Player(){}

		virtual int move(state& board, int &X, int &Y) = 0;

	protected:
		int player;
};

#endif
