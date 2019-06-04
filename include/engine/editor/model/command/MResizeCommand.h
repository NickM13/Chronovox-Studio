#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MResizeCommand : public Command {
public:
	MResizeCommand(Matrix* p_matrix, Matrix* p_fromMatrix);
	void terminate();
	void undo();
	void redo();

private:
	Matrix* m_matrix, *m_fromMatrix, *m_toMatrix;
};
