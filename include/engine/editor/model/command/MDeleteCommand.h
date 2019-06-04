#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"
#include "engine\gfx\gui\LGui.h"

class MDeleteCommand : public Command {
public:
	MDeleteCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList);
	void terminate();
	void undo();
	void redo();

private:
	Matrix* m_matrix;
	std::vector<Matrix*>* m_matrices;
	Uint16 m_index;
	CList* m_nameList;
};
