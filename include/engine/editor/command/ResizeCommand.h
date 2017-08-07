#pragma once

#include "Command.h"
#include "..\model\Matrix.h"

struct ResizeCommand : public Command
{
public:
	ResizeCommand(Matrix* p_matrix, Matrix* p_fromMatrix)
	{
		m_matrix = p_matrix;
		m_fromMatrix = new Matrix(*p_fromMatrix);
		m_toMatrix = new Matrix(*m_matrix);
	}
	~ResizeCommand()
	{
		delete m_fromMatrix;
	}
	void undo()
	{
		*m_matrix = Matrix(*m_fromMatrix);
	}
	void redo()
	{
		*m_matrix = Matrix(*m_toMatrix);
	}

private:
	Matrix* m_matrix, *m_fromMatrix, *m_toMatrix;
};
