#include "gameObjects.h"

const float CPadle::VELOCITY = 0.6f;

void CBall::update(double dt)
{
	glm::vec2 currentV = getVelocity();
	currentV *= dt;

	setPos(m_pos + currentV);

	if (left() < -1.0f && m_velocity.x < 0) m_velocity.x *= -1;
	if (right() > 1.0f  && m_velocity.x > 0) m_velocity.x *= -1;

	if (top() > 1.0f && m_velocity.y > 0) m_velocity.y *= -1;
	if (bottom() < -1.0f && m_velocity.y < 0) m_velocity.y *= -1;
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
		if (abs(minOverlapY) < m_width * 0.2)
		{
			float tempY = abs(v.y);
			float temp = minOverlapY / m_width * 0.2;
			float diff = tempY - tempY * temp;
			v.y = tempY * temp;
			v.x = v.x < 0 ? v.x - diff : v.x + diff;
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


CBoard::CBoard(std::vector<glm::vec2>& points)
{
	m_points = points;
	if (points.size() > 0)
		m_points.push_back(points.at(0));
}

SBoardCollisionDesc CBoard::checkCollision(CBall ball)
{
	glm::vec2 p = ball.getPos();

	float minDist = 0.2;

	for (int i = 1; i < m_points.size(); ++i)
	{
		glm::vec2 a = m_points.at(i - 1);
		glm::vec2 b = m_points.at(i);

		float distA = glm::distance(a, p);
		float distB = glm::distance(b, p);

		if (distA < ball.getRadius() * 0.8)
			return SBoardCollisionDesc(a);

		if (distB < ball.getRadius() * 0.8)
			return SBoardCollisionDesc(b);

		float distAB = glm::distance(a, b);

		if (distA + distB < distAB * 1.0008)
			return SBoardCollisionDesc(a, b);
	}

	return SBoardCollisionDesc();
}

SBoardCollisionDesc CBoard::checkCollision(CPadle padle)
{
	glm::vec2 p = padle.getPos();

	float minDist = 0.2;

	for (int i = 1; i < m_points.size(); ++i)
	{
		glm::vec2 a = m_points.at(i - 1);
		glm::vec2 b = m_points.at(i);

		float distA = glm::distance(a, p);
		float distB = glm::distance(b, p);

		if (distA < padle.getWidth() / 2 * 0.8)
			return SBoardCollisionDesc(a);

		if (distB < padle.getWidth() / 2 * 0.8)
			return SBoardCollisionDesc(b);

		float distAB = glm::distance(a, b);

		if (distA + distB < distAB * 1.0008)
			return SBoardCollisionDesc(a, b);
	}

	return SBoardCollisionDesc();
}

