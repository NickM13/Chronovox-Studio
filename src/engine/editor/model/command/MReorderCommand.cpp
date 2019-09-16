#include "engine\editor\model\command\MReorderCommand.h"
#include "engine\editor\Editor.h"

MReorderCommand::MReorderCommand(std::vector<Matrix*>* p_matrices, std::vector<CList::ListItem> p_order, CList* p_nameList) {
	m_commandType = "ReorderCommand";
	m_matrices = p_matrices;
	m_from = p_order;
	m_to = p_nameList->getItemList();
	m_nameList = p_nameList;
}
void MReorderCommand::terminate() {

}

void MReorderCommand::reorder() {
	Sint32 id = 0;
	for (Sint32 i = id; i < (Sint32)m_matrices->size(); i++) {
		if (m_matrices->at(i)->getId() == id) {
			m_matrices->insert(m_matrices->begin() + id, m_matrices->at(i));
			m_matrices->erase(m_matrices->begin() + i + 1);
			id++;
		}
	}
}

void MReorderCommand::undo() {
	m_nameList->clear();
	for (size_t i = 0; i < m_from.size(); i++) {
		Editor::getModel()->getMatrix(m_from.at(i).name)->setId((Sint16)i);
		m_nameList->addItem(m_from.at(i).name, m_from.at(i).visible, m_from.at(i).state);
	}
	reorder();
}
void MReorderCommand::redo() {
	m_nameList->clear();
	for (size_t i = 0; i < m_from.size(); i++) {
		Editor::getModel()->getMatrix(m_to.at(i).name)->setId((Sint16)i);
		m_nameList->addItem(m_to.at(i).name, m_to.at(i).visible, m_to.at(i).state);
	}
	reorder();
}
