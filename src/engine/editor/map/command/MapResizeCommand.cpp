#include "engine\editor\map\command\MResizeCommand.h"
#include "engine\editor\Editor.h"

MResizeCommand::MResizeCommand(Matrix* p_matrix, Matrix* p_fromMatrix) {
	m_commandType = "ResizeCommand";
	m_matrix = p_matrix->getId();
	m_fromMatrix = new Matrix(*p_fromMatrix);
	m_toMatrix = new Matrix(*p_matrix);
}
void MResizeCommand::terminate() {

}
void MResizeCommand::undo() {
	*Editor::getModel()->getMatrix(m_matrix) = Matrix(*m_fromMatrix);
}
void MResizeCommand::redo() {
	*Editor::getModel()->getMatrix(m_matrix) = Matrix(*m_toMatrix);
}
