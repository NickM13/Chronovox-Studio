#include "engine\editor\model\command\MMoveCommand.h"
#include "engine\editor\Editor.h"

MMoveCommand::MMoveCommand(Matrix* m, glm::vec3 p_from, glm::vec3 p_to) {
	m_commandType = "MoveCommand";
	m_matrix = m->getId();
	from = p_from;
	to = p_to;
}
void MMoveCommand::terminate() {

}

void MMoveCommand::undo() {
	Editor::getModel()->getMatrix(m_matrix)->setPosition(from);
}
void MMoveCommand::redo() {
	Editor::getModel()->getMatrix(m_matrix)->setPosition(to);
}
