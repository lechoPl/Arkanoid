#include "game.h"

#include <glm/gtx/rotate_vector.hpp>

CGame::CGame() 
{
	glm::vec2 A = glm::vec2(0.5f, -1.f);
	glm::vec2 B = glm::vec2(-0.5f, -1.f);

	std::vector<glm::vec2> boardPoints {
		glm::vec2(-1.f, 0.f),
		glm::vec2(-0.5f, 1.f),
		glm::vec2(0.5f, 1.f),
		glm::vec2(1.f, 0.f),
		A,
		B
	};

	m_board = new CBoard(boardPoints, A, B);
	
	//brick size 0.2 x 0.05
	CBrick tempBrick;
	float brickWidth = tempBrick.getWidth();
	float brickHeight = tempBrick.getHeight();

	for (float y = 0.7f; y > 0.2; y -= (brickHeight + 0.005f))
	{
		for (float x = -0.5f; x < 0.5f; x += brickWidth + 0.005f)
		{
			m_bricks.push_back(CBrick(glm::vec2(x, y)));
		}
	}

	m_gameState = GS_STARTGAME;
}

CGame::~CGame()
{
	if (m_board != NULL)
	{
		delete m_board;
		m_board = NULL;
	}
}

void CGame::update(double dt)
{
	if (m_gameState != GS_MOVE)
		return;

	if (m_board->checkLostLive(m_ball, dt).isCollided())
	{
		--m_playerLives;
		m_ball = CBall();

		m_gameState = GS_LOSTLIVE;
		if (m_playerLives == 0)
		{
			fprintf(stdout, "GAME OVER\n");
		}
		return;
	}

	unsigned notDestroyedBricks = 0;
	for (int i = 0; i < m_bricks.size(); ++i)
	{
		m_bricks.at(i).testCollision(m_ball);
		if (!m_bricks.at(i).isDestroyed())
			++notDestroyedBricks;
	}

	if (notDestroyedBricks == 0)
	{
		m_gameState = GS_WON;
		fprintf(stdout, "YOU WIN!\n");
		return;
	}

	m_padle.testCollision(m_ball);

	m_padle.setVelocity(glm::vec2(0, 0));
	if (m_movePadleRight && !m_movePadleLeft)
		m_padle.setVelocity(glm::vec2(CPadle::VELOCITY, 0));
	if (!m_movePadleRight && m_movePadleLeft)
		m_padle.setVelocity(glm::vec2(-CPadle::VELOCITY, 0));

	SBoardCollisionDesc padleColisionDesc = m_board->checkCollision(m_padle, dt);
	if (padleColisionDesc.isCollided())
	{
		bool temp = m_padle.getVelocity().x < 0;

		if (temp && padleColisionDesc.p1->x < m_padle.getPos().x)
			m_padle.setVelocity(glm::vec2(0, 0));
		if (!temp && padleColisionDesc.p1->x > m_padle.getPos().x)
			m_padle.setVelocity(glm::vec2(0, 0));
	}
	m_padle.update(dt);

	SBoardCollisionDesc boardColisionDesc = m_board->checkCollision(m_ball, dt);
	if (boardColisionDesc.isCollided())
	{
		boardColisionDesc.updateBallVelocity(m_ball);
	}
	
	m_ball.update(dt);
}

void CGame::start()
{
	if (m_gameState != GS_LOSTLIVE && m_gameState != GS_STARTGAME)
		return;
	
	fprintf(stdout,"Player lives: %d\n", m_playerLives);

	if (m_playerLives == 0)
	{
		m_gameState = GS_LOSE;
		return;
	}

	m_gameState = GS_MOVE;
}