#pragma once

#include "..\Component.h"

class BackPanel : public Component
{
public:
	BackPanel() {}

	void render()
	{
		Shader::pushMatrixModel();
		Shader::scale(glm::vec3(GScreen::m_screenSize.x / 2, GScreen::m_screenSize.y / 2, 0));
		GBuffer::setColor(color);
		GBuffer::addVertexQuad(-1, -1);
		GBuffer::addVertexQuad(1, -1);
		GBuffer::addVertexQuad(1, 1);
		GBuffer::addVertexQuad(-1, 1);
		Shader::popMatrixModel();
	}
private:
	const Color color = Color(0, 0, 0, 0.25f);
};
