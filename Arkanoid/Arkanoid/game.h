#ifndef GAME
#define GAME

#include "gameObjects.h"

class CGame
{
protected:
	CBall m_ball;
	CBoard* m_board;

public:
	CGame();
	virtual ~CGame();
	virtual void update(double dt);
	virtual const CBall* getBall() const { return &m_ball; }
	virtual const CBoard* const getBoard() const { return m_board; }
};

#endif