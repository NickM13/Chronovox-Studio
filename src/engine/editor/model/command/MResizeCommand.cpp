#include "engine\editor\model\command\MResizeCommand.h"

MResizeCommand::MResizeCommand(Matrix* p_matrix, Matrix* p_fromMatrix) {
	m_commandType = "ResizeCommand";
	m_matrix = p_matrix;
	m_fromMatrix = new Matrix(*p_fromMatrix);
	m_toMatrix = new Matrix(*m_matrix);
}
void MResizeCommand::terminate() {
	delete m_fromMatrix;
	delete m_toMatrix;
}
void MResizeCommand::undo() {
	*m_matrix = Matrix(*m_fromMatrix);
}
void MResizeCommand::redo() {
	*m_matrix = Matrix(*m_toMatrix);
}
