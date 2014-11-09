#ifndef GAME_OBJECTS
#define GAME_OBJECTS

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
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
	GLfloat alpha;
	glm::vec3 v;
	GLfloat r;
};

class CLine
{
	//line: Ax + By + C = 0;
protected:
	GLfloat m_a, m_b, m_c;

public:
	CLine(GLfloat a, GLfloat b, GLfloat c) : m_a(a), m_b(b), m_c(c) { };
	CLine(glm::vec2 p1, glm::vec2 p2);
	bool getIntersectionPoint(const CLine& f, glm::vec2* p); // if point exist returns true;
	GLfloat getAngleBetween(const CLine& f);
};

class CBoard
{
	//six lines
protected:
	std::vector<CLine> bands
};

#endif