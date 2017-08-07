#pragma once

#include "Command.h"
#include "..\model\Matrix.h"

struct MoveCommand : public Command
{
public:
	MoveCommand(Matrix* m, Vector3<GLfloat> p_from, Vector3<GLfloat> p_to)
	{
		m_matrix = m;
		from = p_from;
		to = p_to;
	}
	void undo()
	{
		m_matrix->setPosition(from);
	}
	void redo()
	{
		m_matrix->setPosition(to);
	}

private:
	Matrix* m_matrix;

	Vector3<GLfloat> from, to;
};
