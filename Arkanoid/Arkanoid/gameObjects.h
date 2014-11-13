#ifndef GAME_OBJECTS
#define GAME_OBJECTS

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

class CGameObject
{
protected:
	glm::vec2 m_pos;

public:
	CGameObject(glm::vec2 p) : m_pos(p) { }

	glm::vec2 getPos() const { return m_pos; }
	void setPos(glm::vec2 pos) { m_pos = pos; }

	float x() const { return m_pos.x; }
	float y() const { return m_pos.y; }
};

class CBall : public CGameObject
{
protected:
	float m_radius;
	glm::vec2 m_velocity;

public:
	CBall(glm::vec2 pos = glm::vec2(0.f, 0.f),
		float r = 0.015f,
		float v = 0.6f,
		float startAngle = 180.f);
	glm::vec2 getVelocity() const { return m_velocity; }
	float getRadius() const { return m_radius; }

	void setRadius(float r) { m_radius = r; }
	void setVelocity(glm::vec2 v) { m_velocity = v; }

	float top() const { return y() + m_radius; }
	float bottom() const { return y() - m_radius; }
	float left() const { return x() - m_radius; }
	float right() const { return x() + m_radius; }

	void update(double dt);
};


class CRectangle : public CGameObject
{
protected:
	float m_width;
	float m_height;

public:
	CRectangle(glm::vec2 pos = glm::vec2(0.f, 0.f),
		float width = 0.0f,
		float height = 0.0f) : CGameObject(pos), m_width(width), m_height(height) { }

	virtual float getWidth() const { return m_width; }
	virtual float getHeight() const { return m_height; }
	
	virtual float top() const { return y() + m_height / 2; }
	virtual float bottom() const { return y() - m_height / 2; }
	virtual float left() const { return x() - m_width / 2; }
	virtual float right() const { return x() + m_width / 2; }

	virtual bool isIntersecting(const CBall& ball);
};


class CBrick : public CRectangle
{
protected:
	bool m_isDestroyed;
	
	void destroy() { m_isDestroyed = true; }

public:
	CBrick(glm::vec2 pos = glm::vec2(0.f, 0.f),
		float width = 0.15f,
		float height = 0.08f) : CRectangle(pos, width, height), m_isDestroyed(false) { }
	
	bool isDestroyed() const { return m_isDestroyed; }
	void testCollision(CBall& ball);
};


class CPadle : public CRectangle
{
protected:
	glm::vec2 m_velocity;

public:
	static const float VELOCITY; // val set in "gameObjects.cpp"

	CPadle(glm::vec2 pos = glm::vec2(0.f, -0.8f),
		float width = 0.2f,
		float height = 0.05f) : CRectangle(pos, width, height) { }

	glm::vec2 getVelocity() const { return m_velocity; }
	void setVelocity(glm::vec2 v) { m_velocity = v; }

	void testCollision(CBall& ball);
	void update(double dt);
};


struct SBoardCollisionDesc
{
	//is set: only p1 or p1 and p2
	glm::vec2* p1;
	glm::vec2* p2;

	SBoardCollisionDesc() : p1(NULL), p2(NULL) { }
	SBoardCollisionDesc(glm::vec2 a) : p1(new glm::vec2(a)), p2(NULL) { }
	SBoardCollisionDesc(glm::vec2 a, glm::vec2 b) : p1(new glm::vec2(a)), p2(new glm::vec2(b)) { }
	virtual ~SBoardCollisionDesc();
	bool isCollided() { return p1 != NULL; }
	void updateBallVelocity(CBall& ball);
};


class CBoard
{
protected:
	std::vector<glm::vec2> m_points;
	glm::vec2 m_A;
	glm::vec2 m_B;

public:
	CBoard(std::vector<glm::vec2>& points, glm::vec2 A, glm::vec2 B);
	std::vector<glm::vec2> getPoints() const { return m_points; }
	SBoardCollisionDesc checkLostLive(CBall& ball, double dt);
	SBoardCollisionDesc checkCollision(CBall& ball, double dt);
	SBoardCollisionDesc checkCollision(CPadle& padle, double dt);
};

#endif
