#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"
#include "engine\gfx\gui\LGui.h"

struct MPasteCommand : public Command
{
public:
	MPasteCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList) {
		m_commandType = "PasteCommand";
		m_matrix = p_matrix;
		m_matrices = &p_matrices;
		m_index = p_index;
		m_staticChange = false;
		m_nameList = p_nameList;
	}
	~MPasteCommand() {
		delete m_matrix;
	}
	void undo() {
		m_nameList->removeItem(m_index);
		m_matrices->erase(m_matrices->begin() + m_index);
	}
	void redo() {
		m_nameList->insertItem(m_index, m_matrix->getName());
		m_matrices->insert(m_matrices->begin() + m_index, m_matrix);
	}

private:
	Matrix* m_matrix;
	std::vector<Matrix*>* m_matrices;
	Uint16 m_index;
	CList* m_nameList;
};
