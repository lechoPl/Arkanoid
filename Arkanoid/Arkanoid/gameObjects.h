#ifndef GAME_OBJECTS
#define GAME_OBJECTS

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <vector>

class CGameObject
{
protected:
	glm::vec2 pos;
};

class CBrick : public CGameObject
{
protected:
	GLfloat width;
	GLfloat height;
};

class CBall : public CGameObject
{
protected:
	GLfloat m_angle; // 
	GLfloat m_v;
	GLfloat m_r;
	glm::vec2 m_pos;

public:
	CBall(glm::vec2 pos = glm::vec2(0.f, 0.f),
		GLfloat r = 0.015f,
		GLfloat v = 0.9f,
		GLfloat angle = 0.f //in deg
		) : m_angle(angle), m_v(v), m_r(r), m_pos(pos) { };
	
	GLfloat getAngle() const { return m_angle; }
	GLfloat getV() const { return m_v; }
	GLfloat getR() const { return m_r; }
	glm::vec2 getPos() const { return m_pos; }

	void setAngle(GLfloat a) { m_angle = a; } // need % 360 deg ( 2 PI )
	void setV(GLfloat v) { m_v = v; }
	void setR(GLfloat r) { m_r = r; }
	void setPos(glm::vec2 pos) { m_pos = pos; }

};

/********************************
			BOARD
*********************************/

class CLine
{
	// line: Ax + By + C = 0;
	// A != 0 or B != 0;
protected:
	GLfloat m_a, m_b, m_c;

public:
	CLine(GLfloat a, GLfloat b, GLfloat c) : m_a(a), m_b(b), m_c(c) { };
	CLine(glm::vec2 p1, glm::vec2 p2);
	bool getIntersectionPoint(const CLine& f, glm::vec2* p); // if point exist returns true;
};

struct CBoardEdge
{
	CLine line;
	glm::vec2 vec;

	CBoardEdge(glm::vec2 p1, glm::vec2 p2) : line(p1, p2), vec(p2 - p1) { };
};

class CBoard
{
protected:
	std::vector<glm::vec2> m_points;
	std::vector<CBoardEdge> m_bands;

public:
	CBoard(std::vector<glm::vec2>& points);
	bool checkCollision(const CBall& ball, const glm::vec2& vecV, glm::vec2* p);
	std::vector<glm::vec2> getPoints() const { return m_points; };
};

#endif