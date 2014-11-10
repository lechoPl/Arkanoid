#include "gameObjects.h"

/********************************
			BOARD
*********************************/

CLine::CLine(glm::vec2 p1, glm::vec2 p2)
{
	m_a = p1.y - p2.y;
	m_b = p2.x - p1.x;
	m_c = p1.x * p2.y - p2.x * p1.y;
}

bool CLine::getIntersectionPoint(const CLine& f, glm::vec2* p)
{
	//TO DO: Check this!!!
	if (m_a == f.m_a && m_b == f.m_b)
	{
		// lines are perpendicular;
		// ignore case when to line are the same line;
		return false;
	}

	if (p == NULL)
		p = new glm::vec2();

	if (m_a == 0)
	{
		//f1: By + C = 0 ==> y = -C/B
		p->y = -m_c / m_b;

		//f2: Ax + By + C = 0 ==> x = -(By + C)/A
		p->x = -(f.m_b * p->y + f.m_c) / f.m_a;
	}
	else if (m_b == 0)
	{
		//f1: Ax + C = 0 ==> x = -C/A
		p->x = -m_c / m_a;

		//f2: Ax + By + C = 0 ==> y = -(Ax + C)/B
		p->y = -(f.m_a * p->x + f.m_c) / f.m_b;
	}
	else
	{
		p->y = (m_c * f.m_a - f.m_c * m_a) / (f.m_b * m_a - f.m_a * m_b);
		p->x = -1 * (m_b * p->y + m_c) / m_a;
	}

	return true;
}

CBoard::CBoard(std::vector<glm::vec2>& points)
{
	m_points = points;

	if (m_points.size() > 0)
	{
		for (int i = 1; i < m_points.size(); ++i)
		{
			m_bands.push_back( CBoardEdge(m_points.at(i - 1), m_points.at(i)) );
		}
		m_bands.push_back(CBoardEdge(m_points.at(0), m_points.at(m_points.size() - 1)));
	}
}

bool CBoard::checkCollision(const CBall& ball, const glm::vec2& vecV, glm::vec2* p)
{
	CLine ballLine = CLine(ball.getPos(), ball.getPos() + vecV);
	
	bool isCollided;
	glm::vec2 collisionPoint;
	glm::vec2* minCollisionPoint = NULL;
	float minDist = 0;

	for (int i = 0; i < m_bands.size(); ++i)
	{
		isCollided = m_bands.at(i).line.getIntersectionPoint(ballLine, &collisionPoint);
		if (isCollided)
		{
			float tempDist = glm::distance(ball.getPos(), collisionPoint);
			if (minCollisionPoint == NULL)
			{
				if (tempDist >= 0)
				{
					minCollisionPoint = new glm::vec2(collisionPoint);
					minDist = tempDist;
				}
			}
			else
			{
				if (tempDist >= 0 && tempDist < minDist)
				{
					delete minCollisionPoint;
					minCollisionPoint = new glm::vec2(collisionPoint);
					minDist = tempDist;
				}
			}
		}
	}
	
	if (isCollided)
	{
		if (p == NULL)
			p = new glm::vec2();
		
		p->x = minCollisionPoint->x;
		p->y = minCollisionPoint->y;
	}

	if (minCollisionPoint != NULL)
	{
		delete minCollisionPoint;
		minCollisionPoint = NULL;
	}

	return isCollided;
}