#include "model.h"

std::vector<GLfloat> getVertexBufferData(const CGame& game, SObjectsDrawDesc* objDrawDesc)
{
	if (objDrawDesc == NULL)
		objDrawDesc = new SObjectsDrawDesc();

	unsigned offset = 0;

	std::vector<GLfloat> vertexBufferData;

	if (game.getBoard()->getPoints().size() > 0)
	{
		vertexBufferData.push_back(0.f);
		vertexBufferData.push_back(0.f);
		for (int i = 0; i < game.getBoard()->getPoints().size(); ++i)
		{
			vertexBufferData.push_back(game.getBoard()->getPoints().at(i).x);
			vertexBufferData.push_back(game.getBoard()->getPoints().at(i).y);
		}
	}
	objDrawDesc->boardData = SDrawDesc(offset, vertexBufferData.size() / 2, GL_TRIANGLE_FAN);
	offset = vertexBufferData.size() / 2;

	std::vector<GLfloat> ball{
		-game.getBall()->getRadius(), 0.f,
		0.f, game.getBall()->getRadius(),
		0.f, -game.getBall()->getRadius(),
		game.getBall()->getRadius(), 0.f
	};
	vertexBufferData.insert(vertexBufferData.end(), ball.begin(), ball.end());
	objDrawDesc->ballData = SDrawDesc(offset, ball.size() / 2, GL_TRIANGLE_STRIP);
	offset = vertexBufferData.size() / 2;

	std::vector<GLfloat> rectangle{
		-1.f, 1.f,
		1.f, 1.f,
		-1.f, -1.f,
		1.f, -1.f
	};
	vertexBufferData.insert(vertexBufferData.end(), rectangle.begin(), rectangle.end());
	objDrawDesc->brickData = SDrawDesc(offset, rectangle.size() / 2, GL_TRIANGLE_STRIP);
	objDrawDesc->padleData = SDrawDesc(offset, rectangle.size() / 2, GL_TRIANGLE_STRIP);
	offset = vertexBufferData.size() / 2;

	std::vector<GLfloat> bg{
		-1.f, 0.f,
		-0.5f, 1.f,
		0.5f, 1.f,
		1.f, 0.f,
		0.5f, -1.f,
		-0.5f, -1.f,
		-1.f, 0.f
	};
	vertexBufferData.insert(vertexBufferData.end(), bg.begin(), bg.end());
	objDrawDesc->bgData = SDrawDesc(offset, bg.size() / 2, GL_LINE_STRIP);
	offset = vertexBufferData.size() / 2;

	std::vector<GLfloat> firstPlan{
		-1.f, 1.f,
		-0.5f, 1.f,
		-1.f, 0.f,
		
		-1.f, 0.f,
		-0.5f, -1.f,
		-1.f, -1.f,

		1.f, 0.f,
		0.5f, -1.f,
		1.f, -1.f,

		1.f, 1.f,
		0.5f, 1.f,
		1.f, 0.f,
	};
	vertexBufferData.insert(vertexBufferData.end(), firstPlan.begin(), firstPlan.end());
	objDrawDesc->firstPlan = SDrawDesc(offset, firstPlan.size() / 2, GL_TRIANGLES);
	offset = vertexBufferData.size() / 2;

	return vertexBufferData;
};