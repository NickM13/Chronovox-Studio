#pragma once

#include "Command.h"
#include "..\model\Matrix.h"

struct NewCommand : public Command
{
public:
	NewCommand(Matrix* p_matrix, std::vector<Matrix*>& p_list, Uint16 p_index)
	{
		m_matrix = p_matrix;
		m_list = &p_list;
		m_index = p_index;
		m_staticChange = false;
	}
	~NewCommand()
	{
		delete m_matrix;
	}
	void undo()
	{
		m_list->erase(m_list->begin() + m_index);
	}
	void redo()
	{
		m_list->insert(m_list->begin() + m_index, m_matrix);
	}

private:
	Matrix* m_matrix;
	std::vector<Matrix*>* m_list;
	Uint16 m_index;
};
