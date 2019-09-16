#include "engine\editor\model\command\MPasteCommand.h"

MPasteCommand::MPasteCommand(Matrix* p_matrix, std::vector<Matrix*>& p_matrices, Uint16 p_index, CList* p_nameList) {
	m_commandType = "PasteCommand";
	m_pastedMatrix = new Matrix(*p_matrix);
	m_matrices = &p_matrices;
	m_index = p_index;
	m_staticChange = false;
	m_nameList = p_nameList;
}
void MPasteCommand::terminate() {
	delete m_pastedMatrix;
}
void MPasteCommand::undo() {
	m_nameList->removeItem(m_index);
	m_matrices->erase(m_matrices->begin() + m_index);
}
void MPasteCommand::redo() {
	m_nameList->insertItem(m_index, m_pastedMatrix->getName());
	m_matrices->insert(m_matrices->begin() + m_index, m_pastedMatrix);
}
