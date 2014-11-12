#ifndef GAME
#define GAME

#include "gameObjects.h"

enum GAMESTATE
{
	GS_STARTGAME,
	GS_MOVE,
	GS_WON,
	GS_LOSE,
	GS_LOSTLIVE
};

class CGame
{
protected:
	unsigned m_playerLives = 3;
	GAMESTATE m_gameState;

	CBall m_ball;
	CBoard* m_board;
	std::vector<CBrick> m_bricks;
	CPadle m_padle;
	bool m_movePadleRight = false;
	bool m_movePadleLeft = false;

public:
	CGame();
	~CGame();

	void update(double dt);
	
	const CBall* getBall() const { return &m_ball; }
	const CBoard* const getBoard() const { return m_board; }
	const std::vector<CBrick>& getBricks() const { return m_bricks; }
	const CPadle& getPadle() const { return m_padle; }

	void setMovePadleRight(bool v) { m_movePadleRight = v; }
	void setMovePadleLeft(bool v) { m_movePadleLeft = v; }

	void start();
};

#endif