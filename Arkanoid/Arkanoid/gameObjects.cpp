#include "gameObjects.h"

#include <glm/gtx/rotate_vector.hpp>

const float CPadle::VELOCITY = 0.7f;

CBall::CBall(glm::vec2 pos,
	float r,
	float v,
	float startAngle) : CGameObject(pos), m_radius(r)
{
	m_velocity = glm::rotate(glm::vec2(0, v), startAngle);
}


void CBall::update(double dt)
{
	glm::vec2 currentV = getVelocity();
	currentV *= dt;

	setPos(m_pos + currentV);
}

bool CRectangle::isIntersecting(const CBall& ball)
{
	return right() >= ball.left() && left() <= ball.right()
		&& bottom() <= ball.top() && top() >= ball.bottom();
}


void CBrick::testCollision(CBall& ball)
{
	if (isDestroyed())
		return;

	if (!isIntersecting(ball))
		return;

	this->destroy();

	float overlapLeft = ball.right() - left();
	float overlapRight = right() - ball.left();
	float overlapTop = ball.bottom() - top();
	float overlapBottom = bottom() - ball.top();

	bool fromLeft = abs(overlapLeft) < abs(overlapRight);
	bool fromTop = abs(overlapTop) < abs(overlapBottom);

	float minOverlapX = fromLeft ? overlapLeft : overlapRight;
	float minOverlapY = fromTop ? overlapTop : overlapBottom;

	glm::vec2 v = ball.getVelocity();
	if (abs(minOverlapX) < abs(minOverlapY))
		v.x = fromLeft ? -abs(v.x) : abs(v.x);
	else
		v.y = fromTop ? abs(v.y) : -abs(v.y);


	ball.setVelocity(v);
}

void CPadle::testCollision(CBall& ball)
{
	if (!isIntersecting(ball))
		return;

	float overlapLeft = ball.right() - left();
	float overlapRight = right() - ball.left();
	float overlapTop = ball.bottom() - top();
	float overlapBottom = bottom() - ball.top();

	bool fromLeft = abs(overlapLeft) < abs(overlapRight);
	bool fromTop = abs(overlapTop) < abs(overlapBottom);

	float minOverlapX = fromLeft ? overlapLeft : overlapRight;
	float minOverlapY = fromTop ? overlapTop : overlapBottom;

	glm::vec2 v = ball.getVelocity();
	if (abs(minOverlapX) < abs(minOverlapY))
		v.x = fromLeft ? -abs(v.x) : abs(v.x);
	else if (fromTop)
	{
		float padlePart = 0.3;
		if (abs(minOverlapX) < m_width * padlePart)
		{
			float temp = minOverlapX / (m_width * padlePart);

			const float maxAngle = 70;
			float angle = maxAngle - maxAngle * temp;
			
			if (abs(overlapRight) < abs(overlapLeft))
				angle = -angle;

			v = glm::vec2(v.x, abs(v.y));
			v = glm::rotate(v, angle);
		}
		else
			v.y = abs(v.y);
	}
	else
		v.y = -abs(v.y);


	ball.setVelocity(v);
}

void CPadle::update(double dt)
{
	glm::vec2 currentV = getVelocity();
	currentV *= dt;

	setPos(m_pos + currentV);
}

SBoardCollisionDesc::~SBoardCollisionDesc()
{
	if (p1 != NULL)
	{
		delete p1;
		p1 = NULL;
	}

	if (p2 != NULL)
	{
		delete p2;
		p2 = NULL;
	}
}

void SBoardCollisionDesc::updateBallVelocity(CBall& ball)
{
	if (p1 == NULL)
		return;
	else if (p2 == NULL)
	{
		glm::vec2 v = ball.getVelocity();
		v *= -1;
		ball.setVelocity(v);
		return;
	}
	else
	{
		glm::vec2 temp = *p2 - *p1;
		glm::vec2 n = glm::normalize(glm::vec2(-temp.y, temp.x));

		glm::vec2 u = (glm::dot(ball.getVelocity(), n) / glm::dot(n, n))*n;
		glm::vec2 w = ball.getVelocity() - u;

		ball.setVelocity(w - u);
	}
}


CBoard::CBoard(std::vector<glm::vec2>& points, glm::vec2 A, glm::vec2 B)
{
	m_points = points;
	if (points.size() > 0)
		m_points.push_back(points.at(0));

	m_A = A;
	m_B = B;
}

float cross(glm::vec2 v, glm::vec2 w)
{
	return v.x * w.y - v.y * w.x;
}

SBoardCollisionDesc CBoard::checkLostLive(CBall& ball, double dt)
{
	glm::vec2 q = ball.getPos();
	glm::vec2 s = ball.getVelocity();
	s *= dt;

	glm::vec2 p = m_A;
	glm::vec2 r = m_B - p;

	float t = cross((q - p), s) / cross(r, s);
	float u = cross((q - p), r) / cross(r, s);

	if (u >= 0 && u <= 1 && t >= 0 && t <= 1)
	{
		return SBoardCollisionDesc(p, p + r);
	}

	return SBoardCollisionDesc();
}

SBoardCollisionDesc CBoard::checkCollision(CBall& ball, double dt)
{
	glm::vec2 q = ball.getPos();
	glm::vec2 s = ball.getVelocity();
	s *= dt;

	for (int i = 1; i < m_points.size(); ++i)
	{
		glm::vec2 p = m_points.at(i - 1);
		glm::vec2 r = m_points.at(i) - p;

		float t = cross((q - p), s) / cross(r, s);
		float u = cross((q - p), r) / cross(r, s);

		if (u >= 0 && u <= 1 && t >= 0 && t <= 1)
		{
			return SBoardCollisionDesc(p, p + r);
		}
	}

	//no collision
	return SBoardCollisionDesc();
}

SBoardCollisionDesc CBoard::checkCollision(CPadle& padle, double dt)
{
	glm::vec2 q = padle.getPos();
	glm::vec2 s = padle.getVelocity();
	s *= dt;

	//right
	q.x = padle.right();
	for (int i = 1; i < m_points.size(); ++i)
	{
		glm::vec2 p = m_points.at(i - 1);
		glm::vec2 r = m_points.at(i) - p;
		
		float t = cross((q - p), s) / cross(r, s);
		float u = cross((q - p), r) / cross(r, s);

		if (u >= 0 && u <= 1 && t >= 0 && t <= 1)
		{
			return SBoardCollisionDesc(p, p + r);
		}
	}

	//left
	q.x = padle.left();
	for (int i = 1; i < m_points.size(); ++i)
	{
		glm::vec2 p = m_points.at(i - 1);
		glm::vec2 r = m_points.at(i) - p;
		
		float t = cross((q - p), s) / cross(r, s);
		float u = cross((q - p), r) / cross(r, s);

		if (u >= 0 && u <= 1 && t >= 0 && t <= 1)
		{
			return SBoardCollisionDesc(p, p + r);
		}
	}

	//no collision
	return SBoardCollisionDesc();
}

