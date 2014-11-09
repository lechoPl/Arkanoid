#include "gameObjects.h"

CLine::CLine(glm::vec2 p1, glm::vec2 p2)
{
	m_a = p2.y - p1.y;
	m_b = p2.x - p1.x;
	m_c = p1.x * p2.y + p2.x * p1.y;
}

bool CLine::getIntersectionPoint(const CLine& f, glm::vec2* p)
{
	if (m_a == f.m_a && m_b == f.m_b)
	{
		// lines are perpendicular;
		// ignore case when to line are the same line;
		return false;
	}

	if (p == NULL)
		p = new glm::vec2();

	p->y = (m_c * f.m_a - f.m_c * m_a) / (f.m_b * m_a - f.m_a * m_b);
	p->x = -1 * (m_b * p->y - m_c) / m_a;

	return true;
}

GLfloat CLine::getAngleBetween(const CLine& f)
{
	GLfloat tanVal = abs((m_a - f.m_a) / (1 + m_a*f.m_a));

	return atan(tanVal);
}