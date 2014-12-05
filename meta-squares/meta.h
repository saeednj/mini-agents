#ifndef _META_H_
#define _META_H_

#include "defs.h"
#include "player.h"
#include <sys/time.h>

/** AI Meta Square player */
class Agent : public Player
{
	public:
		Agent(int p, int depth = 4)
			: Player(p), thinkingDepth(depth)
			{
			}
		virtual ~Agent(){}

		/** Processes an AI move */
		virtual int move(state& board, int &X, int &Y);

		long long getThinkingTime();
	private:
		int thinkingDepth;
		struct timeval start, end;

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
		 *  @return Evaluation score of a move
		 */
		int search(state &s, int depth, int alpha, int beta, int sMax, int sMin, int curPlayer, int &X, int &Y);
};


#endif
