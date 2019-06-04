#include "engine\editor\model\command\MNewCommand.h"

MNewCommand::MNewCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList) {
	m_commandType = "NewCommand";
	m_matrix = new Matrix(*p_matrix);
	m_matrices = &p_matrices;
	m_index = p_index;
	m_staticChange = false;
	m_nameList = p_nameList;
}
void MNewCommand::terminate() {
	delete m_matrix;
}
void MNewCommand::undo() {
	m_nameList->removeItem(m_index);
	delete m_matrices->at(m_index);
	m_matrices->erase(m_matrices->begin() + m_index);
}
void MNewCommand::redo() {
	m_nameList->insertItem(m_index, m_matrix->getName());
	m_matrices->insert(m_matrices->begin() + m_index, new Matrix(*m_matrix));
}
