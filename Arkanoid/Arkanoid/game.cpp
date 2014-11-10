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

	m_board = new CBoard(boardPoints);
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
	glm::vec2 v = glm::rotate(glm::vec2(0.f, m_ball.getV() * dt), m_ball.getAngle() );
	
	glm::vec2 p;
	if (!(m_board->checkCollision(m_ball, v, &p)))
	{
		m_ball.setPos(m_ball.getPos() + v);
	}
}