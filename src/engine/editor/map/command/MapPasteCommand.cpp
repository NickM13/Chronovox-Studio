#include "engine\editor\map\command\MPasteCommand.h"

MPasteCommand::MPasteCommand(Chunk* p_matrix, std::vector<Chunk*>& p_matrices, Uint16 p_index, CList* p_nameList) {
	m_commandType = "PasteCommand";
	m_pastedChunk = new Chunk(*p_matrix);
	m_matrices = &p_matrices;
	m_index = p_index;
	m_staticChange = false;
	m_nameList = p_nameList;
}
void MPasteCommand::terminate() {
	delete m_pastedChunk;
}
void MPasteCommand::undo() {
	m_nameList->removeItem(m_index);
	m_matrices->erase(m_matrices->begin() + m_index);
}
void MPasteCommand::redo() {
	m_nameList->insertItem(m_index, m_pastedChunk->getName());
	m_matrices->insert(m_matrices->begin() + m_index, m_pastedChunk);
}
