#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"
#include "engine\gfx\LGui.h"

class MDeleteCommand : public Command {
public:
	MDeleteCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList) {
		m_commandType = "MDeleteCommand";
		m_matrix = p_matrix;
		m_matrices = &p_matrices;
		m_index = p_index;
		m_staticChange = false;
		m_nameList = p_nameList;
	}
	~MDeleteCommand() {
		delete m_matrix;
	}
	void undo() {
		m_nameList->insertItem(m_index, m_matrix->getName());
		m_matrices->insert(m_matrices->begin() + m_index, m_matrix);
	}
	void redo() {
		m_nameList->removeItem(m_index);
		m_matrices->erase(m_matrices->begin() + m_index);
	}

private:
	Matrix* m_matrix;
	std::vector<Matrix*>* m_matrices;
	Uint16 m_index;
	CList* m_nameList;
};
