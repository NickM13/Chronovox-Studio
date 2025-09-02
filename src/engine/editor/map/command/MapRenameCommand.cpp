#include "engine\editor\map\command\MRenameCommand.h"
#include "engine\editor\Editor.h"

MRenameCommand::MRenameCommand(Matrix* m, std::string p_from, std::string p_to) {
	m_commandType = "RenameCommand";
	m_matrix = m->getId();
	from = p_from;
	to = p_to;
}
void MRenameCommand::terminate() {

}

void MRenameCommand::undo() {
	Editor::getModel()->getMatrix(m_matrix)->setName(from);
}
void MRenameCommand::redo() {
	Editor::getModel()->getMatrix(m_matrix)->setName(to);
}
