#ifndef _USER_H_
#define _USER_H_

#include "player.h"

class User : public Player
{
	public:
		User(int p) : Player(p) {}
		virtual ~User(){}

		virtual int move(state& board, int &X, int &Y);
		long long getThinkingTime();
};

#endif
