#include "game.h"

#include <glm/gtx/rotate_vector.hpp>

CGame::CGame() 
{
	std::vector<glm::vec2> boardPoints {
		glm::vec2(-1.f, 0.f),
		glm::vec2(-0.5f, 1.f),
		glm::vec2(0.5f, 1.f),
		glm::vec2(1.f, 0.f),
		glm::vec2(0.5f, -1.f),
		glm::vec2(-0.5f, -1.f)
	};

	float s = 0.9;
	std::vector<glm::vec2> temp;
	for (int i = 0; i < boardPoints.size(); ++i)
	{
		temp.push_back(glm::vec2(boardPoints.at(i).x * s, boardPoints.at(i).y * s));
	}

	m_board = new CBoard(temp);
	
	//brick size 0.2 x 0.05
	for (float y = 0.7; y > 0.2; y -= 0.055)
	{
		for (float x = -0.475; x < 0.5; x += 0.105)
		{
			m_bricks.push_back(CBrick(glm::vec2(x, y)));
		}
	}
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
	SBoardCollisionDesc boardColisionDesc = m_board->checkCollision(m_ball);
	if (boardColisionDesc.isCollided())
	{
		boardColisionDesc.updateBallVelocity(m_ball);
	}

	for (int i = 0; i < m_bricks.size(); ++i)
	{
		m_bricks.at(i).testCollision(m_ball);
	}

	m_padle.testCollision(m_ball);

	m_padle.setVelocity(glm::vec2(0, 0));
	if (m_movePadleRight && !m_movePadleLeft)
		m_padle.setVelocity(glm::vec2(CPadle::VELOCITY, 0));
	if (!m_movePadleRight && m_movePadleLeft)
		m_padle.setVelocity(glm::vec2(-CPadle::VELOCITY, 0));

	SBoardCollisionDesc padleColisionDesc = m_board->checkCollision(m_padle);
	if (!padleColisionDesc.isCollided())
	{
		m_padle.update(dt);
	}

	
	m_ball.update(dt);
}