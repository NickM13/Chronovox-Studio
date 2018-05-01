#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MMoveCommand : public Command
{
public:
	MMoveCommand(Matrix* m, glm::vec3 p_from, glm::vec3 p_to) {
		m_commandType = "MoveCommand";
		m_matrix = m;
		from = p_from;
		to = p_to;
	}
	void undo() {
		m_matrix->setPosition(from);
	}
	void redo() {
		m_matrix->setPosition(to);
	}

private:
	Matrix* m_matrix;

	glm::vec3 from, to;
};
