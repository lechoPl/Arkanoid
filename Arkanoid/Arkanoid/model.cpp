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
		vertexBufferData.push_back(game.getBoard()->getPoints().at(0).x);
		vertexBufferData.push_back(game.getBoard()->getPoints().at(0).y);
	}
	objDrawDesc->boardData = SDrawDesc(offset, vertexBufferData.size() / 2, GL_TRIANGLE_FAN);
	offset = vertexBufferData.size() / 2;

	GLfloat ballR = game.getBall()->getR();
	std::vector<GLfloat> ball{
		-ballR, 0.f,
		0.f, ballR,
		0.f, -ballR,
		ballR, 0.f
	};
	vertexBufferData.insert(vertexBufferData.end(), ball.begin(), ball.end());
	objDrawDesc->ballData = SDrawDesc(offset, ball.size() / 2, GL_TRIANGLE_STRIP);
	offset = vertexBufferData.size() / 2;

	return vertexBufferData;
};