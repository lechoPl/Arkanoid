#ifndef MODEL
#define MODEL

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "game.h"

struct SDrawDesc
{
	unsigned offset;
	unsigned numberOfVertex;
	GLenum mode;

	SDrawDesc() : offset(0), mode(GL_POINT), numberOfVertex(0) { };
	SDrawDesc(unsigned o, unsigned n, GLenum m) : offset(o), mode(m), numberOfVertex(n) { };

	void draw() { glDrawArrays(mode, offset, numberOfVertex); }
};

struct SObjectsDrawDesc
{
	SDrawDesc boardData;
	SDrawDesc brickData;
	SDrawDesc ballData;
	SDrawDesc padleData;
};

std::vector<GLfloat> getVertexBufferData(const CGame& board, SObjectsDrawDesc* objDrawDesc);

#endif
