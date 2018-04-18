#include "engine\editor\model\EditMatrix.h"

EditMatrix::EditMatrix() {
	m_matrix = 0;
	m_initMatrix = 0;
	m_matrixId = -1;
	m_commandListIsOpen = true;
}
EditMatrix::~EditMatrix() {
	clearMatrix(false);
	for(Uint16 i = 0; i < m_recentCommands.size(); i++)
		delete m_recentCommands[i];
	m_recentCommands.clear();
}

void EditMatrix::setCommandListOpen(bool p_isOpen) {
	m_commandListIsOpen = p_isOpen;
}

void EditMatrix::addCommand(Command* p_com) {
	if(!m_commandListIsOpen) return;
	for(Sint16 i = Sint16(m_recentCommands.size()) - 1; i >= m_commandIndex; i--) {
		delete m_recentCommands[i];
		m_recentCommands.erase(m_recentCommands.begin() + i);
	}
	m_recentCommands.push_back(p_com);
	m_commandIndex = Sint16(m_recentCommands.size());
}
void EditMatrix::undo() {
	if(m_commandIndex > 0) {
		Sint16 id = getId();
		reset();
		if(id != -1 && m_recentCommands[m_commandIndex - 1]->isStaticChange()) {

		}
		else
			clearMatrix();
		m_commandIndex--;
		m_recentCommands[m_commandIndex]->undo();
		if(id != -1 && m_recentCommands[m_commandIndex]->isStaticChange()) {
			delete m_initMatrix;
			if(m_matrix)
				m_initMatrix = new Matrix(*m_matrix);
		}
	}
}
void EditMatrix::redo() {
	if(m_commandIndex < m_recentCommands.size()) {
		Sint16 id = getId();
		reset();
		if(m_commandIndex < m_recentCommands.size()) {
			m_recentCommands[m_commandIndex]->redo();
			if(id != -1 && m_recentCommands[m_commandIndex]->isStaticChange()) {
				delete m_initMatrix;
				m_initMatrix = new Matrix(*m_matrix);
			}
			m_commandIndex++;
		}
	}
}
void EditMatrix::clearCommands() {
	for(Command* cmd : m_recentCommands)
		delete cmd;
	m_recentCommands.clear();
	m_commandIndex = 0;
}

void EditMatrix::reset(bool p_saveChanges) {
	if(m_matrix) {
		if(p_saveChanges)
			saveChanges();

		delete m_initMatrix;
		m_initMatrix = new Matrix(*m_matrix);
	}
}

void EditMatrix::setMatrix(Matrix* m, Sint32 id) {
	clearMatrix();

	m_matrixId = id;
	m_matrix = m;
	m_initMatrix = new Matrix(*m);
}
void EditMatrix::clearMatrix(bool p_saveChanges) {
	if(!m_matrix) return;

	if(p_saveChanges && m_matrixId != -1)
		saveChanges();

	delete m_initMatrix;
	m_matrixId = -1;
	m_matrix = 0;
	m_initMatrix = 0;
}
void EditMatrix::saveChanges() {
	bool hasChanged = false;
	if(!(m_matrix->getSize() == m_initMatrix->getSize())) {
		addCommand(new MResizeCommand(m_matrix, m_initMatrix));
	}
	else if(!(m_matrix->getPos() == m_initMatrix->getPos())) {
		addCommand(new MMoveCommand(m_matrix, m_initMatrix->getPos(), m_matrix->getPos()));
	}
	else {
		MDrawCommand* _dcmd = new MDrawCommand(m_matrix);
		for(Sint32 x = 0; x < m_initMatrix->getSize().x; x++) {
			for(Sint32 y = 0; y < m_initMatrix->getSize().y; y++) {
				for(Sint32 z = 0; z < m_initMatrix->getSize().z; z++) {
					if(!(m_matrix->getVoxel({x, y, z}) == getVoxel({x, y, z}))) {
						_dcmd->add({x, y, z}, getVoxel({x, y, z}), m_matrix->getVoxel({x, y, z}));
						hasChanged = true;
					}
				}
			}
		}
		if(hasChanged)
			addCommand(_dcmd);
	}
}
void EditMatrix::refreshPos() {
	if(m_matrix)
		m_initMatrix->setPosition(m_matrix->getPos());
}

Voxel EditMatrix::getVoxel(Vector3<Sint32> p_pos) {
	if(m_initMatrix)
		return m_initMatrix->getVoxel(p_pos);
	return Voxel();
}
Sint16& EditMatrix::getId() {
	return m_matrixId;
}
bool EditMatrix::isSet() {
	return (m_matrix);
}
Vector3<GLfloat> EditMatrix::getPos() {
	if(m_initMatrix)
		return m_initMatrix->getPos();
	return {};
}
Vector3<Sint32> EditMatrix::getSize() {
	if(m_initMatrix)
		return m_initMatrix->getSize();
	return {};
}
Matrix* EditMatrix::getMatrix() {
	return m_matrix;
}
Matrix* EditMatrix::getInitMatrix() {
	return m_initMatrix;
}
