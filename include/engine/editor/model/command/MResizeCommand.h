#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MResizeCommand : public Command
{
public:
	MResizeCommand(Matrix* p_matrix, Matrix* p_fromMatrix) {
		m_commandType = "ResizeCommand";
		m_matrix = p_matrix;
		m_fromMatrix = new Matrix(*p_fromMatrix);
		m_toMatrix = new Matrix(*m_matrix);
	}
	~MResizeCommand() {
		delete m_fromMatrix;
	}
	void undo() {
		*m_matrix = Matrix(*m_fromMatrix);
	}
	void redo() {
		*m_matrix = Matrix(*m_toMatrix);
	}

private:
	Matrix* m_matrix, *m_fromMatrix, *m_toMatrix;
};
