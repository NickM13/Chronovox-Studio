#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MMoveCommand : public Command
{
public:
	MMoveCommand(Matrix* m, glm::vec3 p_from, glm::vec3 p_to);
	void terminate();

	void undo();
	void redo();

private:
	Matrix* m_matrix;

	glm::vec3 from, to;
};
