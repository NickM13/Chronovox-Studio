#include "engine\editor\map\command\MDeleteCommand.h"

MDeleteCommand::MDeleteCommand(Chunk* p_matrix, std::vector<Chunk*>& p_matrices, Uint16 p_index, CList* p_nameList) {
	m_commandType = "MDeleteCommand";
	m_matrix = new Chunk(*p_matrix);
	m_matrices = &p_matrices;
	m_index = p_index;
	m_staticChange = false;
	m_nameList = p_nameList;
}
void MDeleteCommand::terminate() {
	delete m_matrix;
}
void MDeleteCommand::undo() {
	m_nameList->insertItem(m_index, m_matrix->getName());
	m_matrices->insert(m_matrices->begin() + m_index, new Chunk(*m_matrix));
}
void MDeleteCommand::redo() {
	m_nameList->removeItem(m_index);
	delete m_matrices->at(m_index);
	m_matrices->erase(m_matrices->begin() + m_index);
}
