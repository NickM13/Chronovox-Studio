#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MMoveCommand : public Command
{
public:
	MMoveCommand(Matrix* m, Vector3<GLfloat> p_from, Vector3<GLfloat> p_to) {
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

	Vector3<GLfloat> from, to;
};
