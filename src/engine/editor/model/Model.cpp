#include "engine\editor\model\Model.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\variable\manager\ColorManager.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\gfx\gui\Component.h"
#include "engine\gfx\shader\Shader.h"

#include <direct.h>
#include <shlobj.h>
#include <iostream>

Model::Model()
	: TEMode() {
	m_matrixEdit = new EditMatrix();
	MMesh::init();
	MTool::init();
}
Model::~Model() {
	for (Matrix *m : m_matrices)
		delete m;
	m_matrices.clear();
	delete m_matrixEdit;
	MMesh::terminate();
	MTool::terminate();
}
void Model::init() {
	m_matrixCopy = 0;
	m_hoverMatrix = -1;
	m_grid = true;
	m_outline = true;
	m_nameList = (CList*)ModelOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_MATRICES\\LIST_MATRICES");
	m_colorOverlay = (ColorOverlay*)ModelOverlay::getContainer()->findComponent("GUI_DETAILS\\GUI_COLOR\\OVERLAY_COLOR");
	m_colorOverlay->setColorRGB(0, 255, 128);
	m_voxelColor = &m_colorOverlay->getColor();
	if (!autoload()) newModel();
	Tool::init(m_matrixEdit, &m_subTool);
	VoxelTool::init(&m_selectedVoxel, &m_selectedVoxelOffset, &m_selectedSide, m_voxelColor);
	MatrixTool::init(&m_matrices, &m_scalePos, &m_selectedScale);
}

void Model::setDataString(std::string* p_dataString) {
	m_dataString = p_dataString;
}
void Model::setTool(Sint32 p_tool) {
	setSubTool(0);
	m_tool = p_tool;
	ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR\\TOOLBAR")->setSelectedItem(p_tool);
	Tool* tool = MTool::getTool(m_tool);
}
void Model::setTool(std::string p_toolName) {
	setSubTool(0);
	MTool::getTool(m_tool)->disable();
	for (Sint32 i = 0; i < (Sint32)MTool::getToolList().size(); i++)
		if (MTool::getToolList()[i]->getName() == p_toolName) {
			m_tool = i;
			break;
		}
	ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR\\TOOLBAR")->setSelectedItem(m_tool);
	MTool::getTool(m_tool)->enable();
}
void Model::setSubTool(Sint32 p_subTool) {
	if (p_subTool < 0) return;
	Tool* tool = MTool::getTool(m_tool);
	if (tool->hasSubmenu()) {
		bool success = false;
		switch (tool->getType()) {
		case Tool::ToolType::VOXEL:
			success = (p_subTool < static_cast<CButtonRadio*>(ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR_VOXEL\\TOOLBAR"))->getButtonCount());
			ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR_VOXEL\\TOOLBAR")->setSelectedItem(p_subTool);
			break;
		case Tool::ToolType::MATRIX:
			success = (p_subTool < static_cast<CButtonRadio*>(ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR_MATRIX\\TOOLBAR"))->getButtonCount());
			ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR_MATRIX\\TOOLBAR")->setSelectedItem(p_subTool);
			break;
		default: break;
		}
		if (success) m_subTool = p_subTool;
	}
	else m_subTool = 0;
}
void Model::updateTool() {
	m_tool = ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR\\TOOLBAR")->getSelectedItem();
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL)
		m_subTool = ModelOverlay::getContainer()->findComponent("GUI_TOOLBAR_VOXEL\\TOOLBAR")->getSelectedItem();
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::MATRIX) hoverMatrix(-1);
}

void Model::toggleGrid() {
	m_grid = !m_grid;
}
void Model::toggleOutline() {
	m_outline = !m_outline;
}

void Model::focus() {
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
		Camera::setPosition(m_matrixEdit->getPos() + glm::vec3(m_selectedVoxel) + glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else if (MTool::getTool(m_tool)->getType() == Tool::ToolType::MATRIX) {
		Camera::setPosition(getSelectedMatricesCenter());
	}
}

void Model::fixSelectedMatrix() {
	if (m_nameList->getSelectedItem() == -1) {
		m_matrixEdit->clearMatrix();
	}
	else {
		m_matrixEdit->setMatrix(getMatrix(m_nameList->getSelectedItem()), m_nameList->getSelectedItem());
	}
}

void Model::undo() {
	m_matrixEdit->undo();
	fixSelectedMatrix();
}
void Model::redo() {
	m_matrixEdit->redo();
	fixSelectedMatrix();
}

void Model::copyMatrix() {
	if (m_matrixCopy) delete m_matrixCopy;
	if (m_matrixEdit->getId() != -1)
		m_matrixCopy = new Matrix(*m_matrices[m_matrixEdit->getId()]);
}
void Model::pasteMatrix() {
	if (m_matrixCopy) {
		m_matrixEdit->setCommandListOpen(false);
		addMatrix(m_matrixCopy->getName() + " - Copy", Camera::getPosition() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)), m_matrixCopy->getSize());
		Matrix* _matrix = getMatrix(Sint16(getMatrixNames().size()) - 1);
		_matrix->setPosition(Camera::getPosition() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)));
		MNewCommand* _cmd = new MNewCommand(_matrix, m_matrices, Uint16(m_matrices.size()) - 1, m_nameList);
		m_matrixEdit->addCommand(_cmd);
		for (Uint16 x = 0; x < m_matrixCopy->getSize().x; x++) {
			for (Uint16 y = 0; y < m_matrixCopy->getSize().y; y++) {
				for (Uint16 z = 0; z < m_matrixCopy->getSize().z; z++) {
					_matrix->setVoxel({ x, y, z }, m_matrixCopy->getVoxel({ x, y, z }));
				}
			}
		}
		m_matrixEdit->reset();
		m_matrixEdit->setCommandListOpen(true);
		m_matrixEdit->addCommand(new MNewCommand(m_matrices[m_matrices.size() - 1], m_matrices, Uint16(m_matrices.size()) - 1, m_nameList));
	}
}

void Model::setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel) {
	m_matrices[p_matrix]->setVoxel(p_pos, p_voxel);
}
Voxel Model::getVoxel(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxel(p_pos);
}
Uint16 Model::getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_matrices[p_matrix]->getVoxelId(p_pos);
}

void Model::resize(Uint16 p_matrixId, glm::ivec3 p_offset, glm::ivec3 p_size) {
	if (p_size.x <= p_offset.x || p_size.x <= p_offset.x || p_size.x <= p_offset.x) return;
	Sint32 id = m_matrixEdit->getId();
	m_matrixEdit->clearMatrix();
	Matrix* prev = new Matrix(*m_matrices[p_matrixId]);
	m_matrices[p_matrixId]->setSize(p_size);
	m_matrices[p_matrixId]->shiftVoxels(p_offset * -1);
	m_matrices[p_matrixId]->setSize(p_size - p_offset);
	m_matrices[p_matrixId]->addPosition(p_offset);
	MResizeCommand* _cmd = new MResizeCommand(m_matrices[p_matrixId], prev);
	m_matrixEdit->addCommand(_cmd);

	if (id != -1) {
		m_matrixEdit->setMatrix(m_matrices[id], id);
	}
}
void Model::shiftMatrix(glm::ivec3 p_direction) {
	if (m_hoverMatrix != -1) {
		m_matrices[m_hoverMatrix]->shiftVoxels(p_direction);
	}
}
void Model::flipMatrix(Sint8 p_axesFlags) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Matrix* m : getSelectedMatrices()) {
		m_matrixEdit->setMatrix(m, m->getId());
		m->flip(p_axesFlags, center);
		m_matrixEdit->clearMatrix();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedMatrix();
}
void Model::rotateMatrix(Sint8 p_axesFlags) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Matrix* m : getSelectedMatrices()) {
		m_matrixEdit->setMatrix(m, m->getId());
		m->rotate(p_axesFlags, center);
		m_matrixEdit->clearMatrix();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedMatrix();
}
void Model::addMatrix(std::string p_name, glm::vec3 p_pos, glm::ivec3 p_size) {
	bool validName;
	Uint16 i = 0;
	std::string _name = p_name;
	do {
		validName = true;
		for (Matrix *m : m_matrices) {
			if (m->getName() == _name) {
				validName = false;
				_name = p_name + Util::numToStringInt(i);
				i++;
				break;
			}
		}
	} while (!validName);
	Sint16 id = Sint16(m_matrices.size());
	m_matrices.push_back(new Matrix(id, _name, "", p_pos, p_size));
	MNewCommand* _cmd = new MNewCommand(m_matrices[m_matrices.size() - 1], m_matrices, Uint16(m_matrices.size()) - 1, m_nameList);
	m_matrixEdit->addCommand(_cmd);
	m_hoverMatrix = Sint16(m_matrices.size()) - 1;
	m_matrixEdit->setMatrix(m_matrices[m_hoverMatrix], m_hoverMatrix);
	glm::vec3 _pos = m_pos = { 1000, 1000, 1000 };
	m_size = { -1000, -1000, -1000 };
	for (Matrix *m : m_matrices) {
		_pos = glm::vec3(std::fminf(m_pos.x, m->getPos().x), std::fminf(m_pos.y, m->getPos().y), std::fminf(m_pos.z, m->getPos().z));
		m_size = glm::vec3(std::fmaxf(m_pos.x + m_size.x, m->getPos().x + m->getSize().x) - _pos.x,
			std::fmaxf(m_pos.y + m_size.y, m->getPos().y + m->getSize().y) - _pos.y,
			std::fmaxf(m_pos.z + m_size.z, m->getPos().z + m->getSize().z) - _pos.z);
		m_pos = _pos;
	}
	updateMatrixList();
}
void Model::renameMatrix(Uint16 id, std::string p_name) {
	m_matrices[id]->setName(p_name);
	m_nameList->getItem(id).name = p_name;
	updateMatrixList();
}
void Model::deleteSelectedMatrices() {
	std::vector<MDeleteCommand*> _cmd;
	m_matrixEdit->clearMatrix();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		if (m_nameList->getItem(i).state == 2) {
			m_matrixEdit->addCommand(new MDeleteCommand(m_matrices[i], m_matrices, i, m_nameList));
			m_nameList->removeItem(i);
			m_matrices.erase(m_matrices.begin() + i);
		}
	}
	m_matrixEdit->setCommandChaining(false);
	updateMatrixList();
}
void Model::moveMatrix(bool p_up) {
	// Reversed because it is rendered opposite
	if (!p_up) {
		for (Sint32 i = m_nameList->getItemCount() - 2; i >= 0; i--) {
			if (m_nameList->getItem(i).state != 2) continue;
			m_matrices.insert(m_matrices.begin() + i + 2, m_matrices.at(i));
			m_matrices.erase(m_matrices.begin() + i);
			m_nameList->insertItem(i + 2, m_nameList->getItem(i).name);
			m_nameList->getItem(i + 2).state = 2;
			m_nameList->removeItem(i);
		}
	}
	else {
		for (Sint32 i = 1; i < m_nameList->getItemCount(); i++) {
			if (m_nameList->getItem(i).state != 2) continue;
			m_matrices.insert(m_matrices.begin() + i - 1, m_matrices.at(i));
			m_matrices.erase(m_matrices.begin() + i + 1);
			m_nameList->insertItem(i - 1, m_nameList->getItem(i).name);
			m_nameList->getItem(i - 1).state = 2;
			m_nameList->removeItem(i + 1);
		}
	}
	Sint32 id = 0;
	for (Matrix* m : m_matrices) {
		m->setId(id);
		id++;
	}
}
void Model::hoverMatrix(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1 && m_nameList->getItem(m_matrixEdit->getId()).state != 2) {
			m_nameList->getItem(m_matrixEdit->getId()).state = 0;
			if (id == -1) m_matrixEdit->clearMatrix();
		}
	}
	else {
		if (m_matrixEdit->getId() != -1 && m_nameList->getItem(m_matrixEdit->getId()).state != 2)
			m_nameList->getItem(m_matrixEdit->getId()).state = 0;
		if (m_nameList->getItem(id).state != 2)
			m_nameList->getItem(id).state = 1;
		m_matrixEdit->setMatrix(m_matrices[id], id);
	}
}
void Model::selectMatrix(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1) {
			if (m_nameList->getItem(m_matrixEdit->getId()).state == 2)
				m_nameList->selectItem(m_matrixEdit->getId());
			m_matrixEdit->clearMatrix();
		}
	}
	else {
		m_nameList->selectItem(id);
		if (m_nameList->getItem(id).state == 2)
			m_matrixEdit->setMatrix(m_matrices[id], id);
		else
			m_matrixEdit->clearMatrix();
	}
}

void Model::updateMatrixList() {
	m_nameList->clear();
	Sint32 id = 0;
	for (Matrix* m : m_matrices) {
		m->setId(id);
		m_nameList->addItem(m->getName());
		id++;
	}
}

void Model::inputEditor(Sint8 p_guiFlags) {
	bool mouseOnGui = !(p_guiFlags & (Sint8)Component::EventFlag::MOUSEOVER);
	GLfloat _near = mouseOnGui ? 1 : 0, _far = 1;
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL
		&& GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)
		&& m_matrixEdit->getId() != -1)
		ModelMath::castRayMatrix(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), m_matrixEdit->getInitMatrix(), _near, _far);
	else
		ModelMath::castRayMatrices(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), m_matrices, m_hoverMatrix, _near, _far);

	if (!mouseOnGui) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			if ((!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) && m_hoverMatrix != m_matrixEdit->getId()) || GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT))
				hoverMatrix(m_hoverMatrix);
			break;
		case Tool::ToolType::MATRIX:
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT))
				if (m_matrixEdit->getId() == -1 || m_selectedScale == 0)
					selectMatrix(m_hoverMatrix);
			break;
		default: break;
		}
	}

	if (!mouseOnGui && m_matrixEdit->getId() != -1 && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT)) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			ModelMath::castRayVoxel(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), m_matrixEdit->getInitMatrix(), _near, _far, m_selectedVoxel, m_selectedSide, m_selectedVoxelOffset);
			static_cast<VoxelTool*>(MTool::getTool(m_tool))->input();
			break;
		case Tool::ToolType::MATRIX:
			ModelMath::castRayScale(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), m_matrixEdit->getMatrix(), _near, _far, m_scalePos, m_selectedScale);
			static_cast<MatrixTool*>(MTool::getTool(m_tool))->input();
			break;
		}
	}
	else {
		m_selectedVoxel = m_selectedVoxelOffset = { -1, -1, -1 };
	}

	if (!EditorOverlay::getContainer()->isPaused() && (p_guiFlags & (Sint8)Component::EventFlag::KEYPRESS)) {
		if (GKey::keyPressed(GLFW_KEY_Z, GLFW_MOD_CONTROL))       undo();
		if (GKey::keyPressed(GLFW_KEY_C, GLFW_MOD_CONTROL))       copyMatrix();
		if (GKey::keyPressed(GLFW_KEY_V, GLFW_MOD_CONTROL))       pasteMatrix();
		if (GKey::keyPressed(GLFW_KEY_Y, GLFW_MOD_CONTROL))       redo();
		if (GKey::keyPressed(GLFW_KEY_S, GLFW_MOD_CONTROL))       save();
		if (GKey::keyPressed(GLFW_KEY_O, GLFW_MOD_CONTROL))       open();
		if (GKey::keyPressed(GLFW_KEY_G, GLFW_MOD_CONTROL))       toggleGrid();
		if (GKey::keyPressed(GLFW_KEY_H, GLFW_MOD_CONTROL))       toggleOutline();
		if (GKey::keyPressed(GLFW_KEY_N, GLFW_MOD_CONTROL))       ModelOverlay::getContainer()->setPauseScreen("DIALOG_NEWMATRIX");
		if (GKey::keyPressed(GLFW_KEY_F2) && getSelectedMatrix()) ModelOverlay::getContainer()->setPauseScreen("DIALOG_PROPERTIES");
		if (GKey::keyPressed(GLFW_KEY_DELETE))                    deleteSelectedMatrices();
		if (GKey::keyPressed(GLFW_KEY_SPACE))                     focus();
		if (GKey::keyPressed(GLFW_KEY_T))                         setTool("VTAdd");
		if (GKey::keyPressed(GLFW_KEY_E))                         setTool("VTErase");
		if (GKey::keyPressed(GLFW_KEY_R))                         setTool("VTReplace");
		if (GKey::keyPressed(GLFW_KEY_K))                         setTool("VTEyedrop");
		if (GKey::keyPressed(GLFW_KEY_M))                         setTool("MTSelect");
		if (GKey::keyPressed(GLFW_KEY_1))                         setSubTool(0);
		if (GKey::keyPressed(GLFW_KEY_2))                         setSubTool(1);
		if (GKey::keyPressed(GLFW_KEY_3))                         setSubTool(2);
	}
}
void Model::updateEditor(GLfloat p_deltaUpdate) {
	switch (MTool::getTool(m_tool)->getType()) {
	case Tool::ToolType::VOXEL:
		static_cast<VoxelTool*>(MTool::getTool(m_tool))->update();
		break;
	case Tool::ToolType::MATRIX:
		static_cast<MatrixTool*>(MTool::getTool(m_tool))->update();
		break;
	default: break;
	}

	for (Matrix* m : m_matrices)
		m->update();

	Matrix* m = 0;
	if (getSelectedMatrix()) m = getSelectedMatrix();
	else if (m_matrixEdit->getId() != -1) m = m_matrixEdit->getMatrix();
	if (m) {
		*m_dataString = std::string(Util::numToStringInt(m->getId()) +
			": " + m->getName() +
			", {" + Util::numToStringFloat(m->getPos().x + m->getSize().x / 2.f, 1) +
			", " + Util::numToStringFloat(m->getPos().y + m->getSize().y / 2.f, 1) +
			", " + Util::numToStringFloat(m->getPos().z + m->getSize().z / 2.f, 1) +
			"} : {" + Util::numToStringInt(m->getSize().x) +
			", " + Util::numToStringInt(m->getSize().y) +
			", " + Util::numToStringInt(m->getSize().z) + "}");
	}
	else
		*m_dataString = "";
}
void Model::renderEditor() {
	Shader::setLightEnabled(true);
	for (Matrix* m : m_matrices)
		m->renderMatrix();

	Shader::setLightEnabled(false);
	if (m_grid) renderGrid();

	for (Uint16 i = 0; i < m_matrices.size(); i++) {
		if (m_outline)
			m_matrices[i]->renderOutline(static_cast<OutlineType>(m_nameList->getItem(i).state + 1));
		else
			m_matrices[i]->renderOutline(OutlineType::NONE);
	}

	switch (MTool::getTool(m_tool)->getType()) {
	case Tool::ToolType::VOXEL:
		static_cast<VoxelTool*>(MTool::getTool(m_tool))->render();
		break;
	case Tool::ToolType::MATRIX:
		static_cast<MatrixTool*>(MTool::getTool(m_tool))->render();
		break;
	default: break;
	}
}
void Model::renderEditorShadow() {
	for (Matrix* m : m_matrices)
		m->renderShadow();
}
void Model::renderGrid() {
	GLfloat w1 = 0.05f, w2 = w1 / 2.5f;
	GLfloat gc = 32; // Grid Count
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), glm::vec3(0.f, -0.025f, 0.f)));
	Shader::applyModel();
	Shader::setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	for (Sint16 i = -gc; i <= gc; i++) {
		if (i == 0) {
			MMesh::renderLine(glm::vec3(-gc, 0, 0), glm::vec3(0, 0, 0), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, -gc), glm::vec3(0, 0, 0), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(gc, 0, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, gc, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, gc), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else {
			if (i % 2 == 0) {
				MMesh::renderLine(glm::vec3(i, 0, -gc), glm::vec3(i, 0, gc), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
				MMesh::renderLine(glm::vec3(-gc, 0, i), glm::vec3(gc, 0, i), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			}
			else {
				MMesh::renderLine(glm::vec3(i, 0, -gc), glm::vec3(i, 0, gc), glm::vec4(0.5f, 0.5f, 0.5f, 0.35f));
				MMesh::renderLine(glm::vec3(-gc, 0, i), glm::vec3(gc, 0, i), glm::vec4(0.5f, 0.5f, 0.5f, 0.35f));
			}
		}
	}
	Shader::popMatrixModel();
}

std::vector<std::string> Model::getMatrixNames() {
	std::vector<std::string> names;
	for (Uint16 i = 0; i < m_matrices.size(); i++)
		names.push_back("(" + Util::numToStringInt(i) + ") " + m_matrices[i]->getName());
	return names;
}
Matrix* Model::getMatrix(Sint16 id) {
	if (id < 0 || id >= Sint16(m_matrices.size())) return 0;
	return m_matrices[id];
}
Matrix* Model::getMatrix(std::string p_name) {
	for (Matrix* m : m_matrices) {
		if (m->getName() == p_name) return m;
	}
	return 0;
}
Matrix* Model::getSelectedMatrix() {
	if (m_nameList->getSelectedItem() != -1)
		return getMatrix(m_nameList->getSelectedItem());
	return 0;
}
std::vector<Matrix*> Model::getSelectedMatrices() {
	std::vector<Matrix*> selected;
	for (auto li : m_nameList->getItemList())
		if (li.state == 2)
			selected.push_back(getMatrix(li.name));
	return selected;
}
glm::vec3 Model::getSelectedMatricesCenter() {
	GLfloat bn = 1e8;
	glm::vec3 minBound = { bn, bn, bn }, maxBound = { -bn, -bn, -bn };
	glm::vec3 center;
	for (Matrix* m : getSelectedMatrices()) {
		minBound.x = std::fminf(m->getPos().x, minBound.x);
		minBound.y = std::fminf(m->getPos().y, minBound.y);
		minBound.z = std::fminf(m->getPos().z, minBound.z);
		maxBound.x = std::fmaxf(m->getPos().x + m->getSize().x, maxBound.x);
		maxBound.y = std::fmaxf(m->getPos().y + m->getSize().y, maxBound.y);
		maxBound.z = std::fmaxf(m->getPos().z + m->getSize().z, maxBound.z);
	}
	center.x = minBound.x + (maxBound.x - minBound.x) / 2;
	center.y = minBound.y + (maxBound.y - minBound.y) / 2;
	center.z = minBound.z + (maxBound.z - minBound.z) / 2;
	return center;
}

bool Model::exitSave() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Voxel Models");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Nick's Voxel Model\0*.nvm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save As";

	if (!GetSaveFileName(&ofn)) return false;
	save(filename);
	return true;
}
void Model::autosave() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Voxel Models\\autosave.nvm");
	NvmFormat::save(documents, m_matrices);
}
bool Model::autoload() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Voxel Models\\autosave.nvm");
	return load(documents);
}
void Model::save() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Voxel Models");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "NVM (*.nvm)\0*.nvm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save As";
	if (!GetSaveFileName(&ofn)) return;
	m_modelName = filename;
	save(m_modelName);
}
void Model::open() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Voxel Models");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "NVM (*.nvm)\0*.nvm*\0QBCL (*.qbcl)\0*.qbcl*\0QB (*.qb)\0*.qb*\0VOX (*.vox)\0*.vox*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Model";
	if (!GetOpenFileName(&ofn)) return;
	m_modelName = filename;
	load(m_modelName);
}
void Model::newModel() {
	if (m_matrices.size() > 0) {
		save();
		for (Matrix* m : m_matrices)
			delete m;
		m_matrices.clear();
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearMatrix(false);
		m_nameList->clear();
	}
	addMatrix("Matrix", { -5, 0, -5 }, { 10, 10, 10 });
}
void Model::save(std::string p_fileName) {
	Format::save(p_fileName, m_matrices);
}
bool Model::load(std::string p_fileName) {
	Format::FormatType _formatType = Format::valid(p_fileName);
	if (_formatType != Format::FormatType::NONE) {
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearMatrix(false);
		m_nameList->clear();
		m_matrices.clear();
		Format::load(p_fileName, m_matrices, _formatType);
		updateMatrixList();
		return true;
	}
	return false;
}

void Model::fileNew() {
	std::vector<MDeleteCommand*> _cmd;
	m_matrixEdit->clearMatrix();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		m_matrixEdit->addCommand(new MDeleteCommand(m_matrices[i], m_matrices, i, m_nameList));
		m_nameList->removeItem(i);
		m_matrices.erase(m_matrices.begin() + i);
	}
	m_matrixEdit->setCommandChaining(false);
	updateMatrixList();
}
void Model::fileOpen() {
	open();
}
void Model::fileSave() {
	save();
}
void Model::fileExit() {

}

void Model::editUndo() {
	undo();
	updateMatrixList();
}
void Model::editRedo() {
	redo();
	updateMatrixList();
}
