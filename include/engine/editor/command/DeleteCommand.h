#pragma once

#include "Command.h"
#include "..\model\Matrix.h"

struct DeleteCommand : public Command
{
public:
	DeleteCommand(Matrix* p_matrix, std::vector<Matrix*>& p_list, Uint16 p_index)
	{
		m_matrix = p_matrix;
		m_list = &p_list;
		m_index = p_index;
		m_staticChange = false;
	}
	~DeleteCommand()
	{
		delete m_matrix;
	}
	void undo()
	{
		m_list->insert(m_list->begin() + m_index, m_matrix);
	}
	void redo()
	{
		m_list->erase(m_list->begin() + m_index);
	}

private:
	Matrix* m_matrix;
	std::vector<Matrix*>* m_list;
	Uint16 m_index;
};
