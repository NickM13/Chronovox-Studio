#include "engine\editor\model\command\MNewCommand.h"
#include "engine\editor\Editor.h"

MNewCommand::MNewCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList) {
	m_commandType = "NewCommand";
	m_matrix.name = p_matrix->getName();
	m_matrix.pos = p_matrix->getPos();
	m_matrix.size = p_matrix->getSize();
	m_matrices = &p_matrices;
	m_index = p_index;
	m_staticChange = false;
	m_nameList = p_nameList;
}
void MNewCommand::terminate() {
	
}
void MNewCommand::undo() {
	m_nameList->removeItem(m_index);
	delete m_matrices->at(m_index);
	m_matrices->erase(m_matrices->begin() + m_index);
}
void MNewCommand::redo() {
	m_nameList->insertItem(m_index, m_matrix.name);
	m_matrices->insert(m_matrices->begin() + m_index, new Matrix(m_index, m_matrix.name, "", m_matrix.pos, m_matrix.size));
}
