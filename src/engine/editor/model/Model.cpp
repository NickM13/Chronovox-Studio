#include "engine\editor\model\Model.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\variable\manager\ColorManager.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"
#include "engine\gfx\gui\component\Component.h"
#include "engine\utils\global\GLua.h"

#include <direct.h>
#include <shlobj.h>
#include <iostream>
#include <set>

Sint32 Model::m_tool;

glm::ivec3 Model::m_selectedVoxel;
glm::ivec3 Model::m_selectedVoxelOffset;
Sint8 Model::m_selectedSide;
Color* Model::m_voxelColor;
GLfloat Model::m_scriptPos[3];
Sint32 Model::m_scriptSize[3];
Sint32 Model::m_scriptColor[3];
Matrix* Model::m_scriptMatrix;

Model::Model()
	: TEMode() {
	m_sModel = new SimpleModel();
	m_matrixEdit = new EditMatrix();
}
Model::~Model() {
	delete m_sModel;
	delete m_matrixEdit;
}
void Model::init() {
	m_name = "New Model";
	m_directory = "";
	m_matrixCopy = 0;
	m_hoverMatrix = -1;
	m_grid = true;
	m_outline = true;
	m_wireframe = false;
	m_nameList = (CList*)ModelOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_MATRICES\\LIST_MATRICES");
	m_colorOverlay = (ColorOverlay*)ModelOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_COLOR\\OVERLAY_COLOR");
	m_colorOverlay->setColorRGB(0, 255, 128);
	m_voxelColor = &m_colorOverlay->getColor();
	newModel();
	GLua::getGlobal()
		.beginNamespace("color")
		.addVariable("r", &m_scriptColor[0], true)
		.addVariable("g", &m_scriptColor[1], true)
		.addVariable("b", &m_scriptColor[2], true);

	GLua::getGlobal()
		.beginNamespace("pos")
		.addVariable("x", &m_scriptPos[0], true)
		.addVariable("y", &m_scriptPos[1], true)
		.addVariable("z", &m_scriptPos[2], true);

	GLua::getGlobal()
		.beginNamespace("size")
		.addVariable("width", &m_scriptSize[0], true)
		.addVariable("height", &m_scriptSize[1], true)
		.addVariable("depth", &m_scriptSize[2], true);

	GLua::getGlobal().addFunction("addMatrix", &addScriptMatrix);
	GLua::getGlobal().addFunction("getMatrixByName", &getScriptMatrixByName);
	GLua::getGlobal().addFunction("setMatrixPos", &setScriptMatrixPos);
	GLua::getGlobal().addFunction("setMatrixSize", &setScriptMatrixSize);
	GLua::getGlobal().addFunction("setSelectedMatrix", &setScriptMatrixSelected);

	GLua::getGlobal().addFunction("useColor", &useScriptColorPalette);
	GLua::getGlobal().addFunction("getColor", &getScriptColor);
	GLua::getGlobal().addFunction("setVoxel", &setScriptVoxel);
}
void Model::activate() {
	Tool::init(m_matrixEdit);
	VoxelTool::init(&m_selectedVoxel, &m_selectedVoxelOffset, &m_selectedSide, m_voxelColor);
	MatrixTool::init(m_sModel->getMatrixList(), &m_scalePos, &m_selectedScale);
}

bool Model::hasChanged() {
	return m_matrixEdit->hasChanged();
}
void Model::setChanged(bool p_changed) {
	m_matrixEdit->setChanged(p_changed);
}

void Model::setDataString(std::string* p_dataString) {
	m_dataString = p_dataString;
}
void Model::setTool(Sint32 p_tool) {
	m_tool = p_tool;
	ModelOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN")->setSelectedItem(p_tool);
	Tool* tool = MTool::getTool(m_tool);
}
void Model::setTool(std::string p_toolName) {
	if (p_toolName == MTool::getTool(m_tool)->getName()) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			break;
		case Tool::ToolType::MATRIX:
			break;
		default: break;
		}
	}
	else {
		MTool::getTool(m_tool)->disable();
		for (Sint32 i = 0; i < (Sint32)MTool::getToolList().size(); i++) {
			if (MTool::getToolList()[i]->getName() == p_toolName) {
				m_tool = i;
				break;
			}
		}
		ModelOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN")->setSelectedItem(m_tool);
		MTool::getTool(m_tool)->enable();
	}
}
void Model::updateTool() {
	m_tool = MTool::getToolId(static_cast<CButtonRadio*>(ModelOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN"))->getSelectedRadio());
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
	}
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::MATRIX) {
		hoverMatrix(-1);
	}
}

void Model::focus() {
	if (m_matrixEdit->getId() != -1) {
		if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
			if (m_matrixEdit->getMatrix()->containsPoint(m_selectedVoxel)) {
				Camera::setPosition(m_matrixEdit->getPos() + glm::vec3(m_selectedVoxel) + glm::vec3(0.5f, 0.5f, 0.5f));
			}
			else {
				Camera::setPosition(m_matrixEdit->getMatrix()->getCenter());
			}
		}
		else if (MTool::getTool(m_tool)->getType() == Tool::ToolType::MATRIX) {
			Camera::setPosition(getSelectedMatricesCenter());
		}
	}
	else {
		Camera::setPosition(glm::vec3());
	}
}

void Model::fixSelectedMatrix() {
	for (Sint32 i = 0; i < (Sint32)m_nameList->getItemList().size(); i++) {
		auto li = m_nameList->getItemList().at(i);
		getMatrix(li.name)->setId(i);
	}
	if (m_nameList->getSelectedItem() == -1) {
		m_matrixEdit->clearMatrix();
	}
	else {
		m_matrixEdit->setMatrix(getMatrix(m_nameList->getSelectedItem()), m_nameList->getSelectedItem());
	}
}

void Model::undo() {
	m_matrixEdit->undo();
	updateMatrixList();
	fixSelectedMatrix();
}
void Model::redo() {
	m_matrixEdit->redo();
	updateMatrixList();
	fixSelectedMatrix();
}

void Model::copyMatrix() {
	if (m_matrixCopy) delete m_matrixCopy;
	if (m_matrixEdit->getId() != -1)
		m_matrixCopy = new Matrix(*m_sModel->getMatrix(m_matrixEdit->getId()));
}
void Model::pasteMatrix() {
	if (m_matrixCopy) {
		m_matrixEdit->setCommandListOpen(false);
		addMatrix(m_matrixCopy->getName() + " - Copy", Camera::getPosition() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)), m_matrixCopy->getSize());
		Matrix* _matrix = getMatrix(Sint16(getMatrixNames().size()) - 1);
		_matrix->setPosition(Camera::getFocus() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)));
		MNewCommand* _cmd = new MNewCommand(_matrix, *m_sModel->getMatrixList(), Uint16(m_sModel->getMatrixList()->size()) - 1, m_nameList);
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
		m_matrixEdit->addCommand(new MNewCommand(m_sModel->getMatrix(Sint16(m_sModel->getMatrixList()->size()) - 1), *m_sModel->getMatrixList(), Uint16(m_sModel->getMatrixList()->size()) - 1, m_nameList));
	}
}

void Model::setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel) {
	m_sModel->setVoxel(p_matrix, p_pos, p_voxel);
}
Voxel Model::getVoxel(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_sModel->getVoxel(p_matrix, p_pos);
}
Uint16 Model::getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_sModel->getVoxelId(p_matrix, p_pos);
}

void Model::resize(Uint16 p_matrixId, glm::ivec3 p_offset, glm::ivec3 p_size) {
	if (p_size.x <= p_offset.x || p_size.y <= p_offset.y || p_size.z <= p_offset.z) return;
	Sint32 id = m_matrixEdit->getId();
	m_matrixEdit->clearMatrix();
	Matrix* prev = new Matrix(*m_sModel->getMatrix(p_matrixId));
	m_sModel->getMatrix(p_matrixId)->setSize(p_size);
	m_sModel->getMatrix(p_matrixId)->shiftVoxels(p_offset * -1);
	m_sModel->getMatrix(p_matrixId)->setSize(p_size - p_offset);
	m_sModel->getMatrix(p_matrixId)->addPosition(p_offset);
	MResizeCommand* _cmd = new MResizeCommand(m_sModel->getMatrix(p_matrixId), prev);
	m_matrixEdit->addCommand(_cmd);

	if (id != -1) {
		m_matrixEdit->setMatrix(m_sModel->getMatrix(id), id);
	}
}
void Model::shiftMatrix(glm::ivec3 p_direction) {
	if (m_hoverMatrix != -1) {
		m_sModel->getMatrix(m_hoverMatrix)->shiftVoxels(p_direction);
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
void Model::scaleMatrix(glm::vec3 p_scale) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Matrix* m : getSelectedMatrices()) {
		m_matrixEdit->setMatrix(m, m->getId());
		m->scale(p_scale, center);
		m_matrixEdit->clearMatrix();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedMatrix();
}
Sint32 Model::addMatrix(std::string p_name, glm::vec3 p_pos, glm::ivec3 p_size) {
	if (p_name == "") p_name = "Matrix";
	bool validName;
	Uint16 i = 0;
	std::string _name = p_name;
	do {
		validName = true;
		for (Matrix *m : *m_sModel->getMatrixList()) {
			if (m->getName() == _name) {
				validName = false;
				_name = p_name + Util::numToStringInt(i);
				i++;
				break;
			}
		}
	} while (!validName);
	Sint16 id = Sint16(m_sModel->getMatrixList()->size());
	Matrix* matrix = new Matrix(id, _name, "", p_pos, p_size);
	m_sModel->getMatrixList()->push_back(matrix);
	MNewCommand* _cmd = new MNewCommand(m_sModel->getMatrix(Sint16(m_sModel->getMatrixList()->size()) - 1), *m_sModel->getMatrixList(), Uint16(m_sModel->getMatrixList()->size()) - 1, m_nameList);
	m_matrixEdit->addCommand(_cmd);
	m_hoverMatrix = Sint16(m_sModel->getMatrixList()->size()) - 1;
	m_matrixEdit->setMatrix(m_sModel->getMatrix(m_hoverMatrix), m_hoverMatrix);
	glm::vec3 _pos = m_pos = { 1000, 1000, 1000 };
	m_size = { -1000, -1000, -1000 };
	for (Matrix *m : *m_sModel->getMatrixList()) {
		_pos = glm::vec3(std::fminf(m_pos.x, m->getPos().x),
			std::fminf(m_pos.y, m->getPos().y),
			std::fminf(m_pos.z, m->getPos().z));
		m_size = glm::vec3(std::fmaxf(m_pos.x + m_size.x, m->getPos().x + m->getSize().x) - _pos.x,
			std::fmaxf(m_pos.y + m_size.y, m->getPos().y + m->getSize().y) - _pos.y,
			std::fmaxf(m_pos.z + m_size.z, m->getPos().z + m->getSize().z) - _pos.z);
		m_pos = _pos;
	}
	updateMatrixList();
	return m_sModel->getMatrixList()->size() - 1;
}
void Model::renameMatrix(Uint16 id, std::string p_name) {
	if (p_name == "") p_name = "Matrix";
	for (Matrix* m : *m_sModel->getMatrixList()) {
		if (m->getId() != id && m->getName() == p_name) {
			Logger::logQuiet("Could not rename matrix: Name already taken");
			return;
		}
	}
	m_sModel->getMatrix(id)->setName(p_name);
	m_nameList->getItem(id).name = p_name;
	updateMatrixList();
}
void Model::deleteSelectedMatrices() {
	m_matrixEdit->clearMatrix();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		if (m_nameList->getItem(i).state == 2) {
			m_matrixEdit->addCommand(new MDeleteCommand(m_sModel->getMatrix(i), *m_sModel->getMatrixList(), i, m_nameList));
			m_nameList->removeItem(i);
			delete m_sModel->getMatrixList()->at(i);
			m_sModel->getMatrixList()->erase(m_sModel->getMatrixList()->begin() + i);
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
			m_sModel->getMatrixList()->insert(m_sModel->getMatrixList()->begin() + i + 2, m_sModel->getMatrix(i));
			m_sModel->getMatrixList()->erase(m_sModel->getMatrixList()->begin() + i);
			m_nameList->insertItem(i + 2, m_nameList->getItem(i).name);
			m_nameList->getItem(i + 2).state = 2;
			m_nameList->removeItem(i);
		}
	}
	else {
		for (Sint32 i = 1; i < m_nameList->getItemCount(); i++) {
			if (m_nameList->getItem(i).state != 2) continue;
			m_sModel->getMatrixList()->insert(m_sModel->getMatrixList()->begin() + i - 1, m_sModel->getMatrix(i));
			m_sModel->getMatrixList()->erase(m_sModel->getMatrixList()->begin() + i + 1);
			m_nameList->insertItem(i - 1, m_nameList->getItem(i).name);
			m_nameList->getItem(i - 1).state = 2;
			m_nameList->removeItem(i + 1);
		}
	}
	Sint32 id = 0;
	for (Matrix* m : *m_sModel->getMatrixList()) {
		m->setId(id);
		id++;
	}
}
void Model::hoverMatrix(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1) {
			if (m_nameList->getItem(m_matrixEdit->getId()).state != 2) {
				m_nameList->getItem(m_matrixEdit->getId()).state = 0;
				m_matrixEdit->clearMatrix();
			}
			else {
				m_matrixEdit->reset();
			}
		}
	}
	else {
		if (m_matrixEdit->getId() != -1 && m_nameList->getItem(m_matrixEdit->getId()).state != 2)
			m_nameList->getItem(m_matrixEdit->getId()).state = 0;
		if (m_nameList->getItem(id).state != 2)
			m_nameList->getItem(id).state = 1;
		m_matrixEdit->setMatrix(m_sModel->getMatrix(id), id);
	}
}
void Model::setSelectedMatrix(Sint16 id) {
	m_matrixEdit->clearMatrix();
	if (id != -1) {
		m_matrixEdit->setMatrix(getMatrix(id), id);
	}
}
void Model::selectMatrix(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1) {
			if (m_nameList->getItem(m_matrixEdit->getId()).state == 2) {
				m_nameList->selectItem(m_matrixEdit->getId());
			}
			m_matrixEdit->clearMatrix();
		}
	}
	else {
		if (m_nameList->getItem(id).state != 2) {
			m_nameList->selectItem(id);
		}
		if (m_nameList->getItem(id).state == 2) {
			m_matrixEdit->setMatrix(m_sModel->getMatrix(id), id);
		}
		else {
			m_matrixEdit->clearMatrix();
			Sint32 i = 0;
			for (auto li : m_nameList->getItemList()) {
				if (li.state == 2) {
					m_nameList->setSelectedItem(i);
					m_matrixEdit->setMatrix(m_sModel->getMatrix(i), i);
					break;
				}
				i++;
			}
		}
	}
}

void Model::updateMatrixList() {
	std::set<std::string> selectSet;
	for (auto li : m_nameList->getItemList()) {
		if (li.state == 2)
			selectSet.insert(li.name);
	}
	std::string selected = "";
	if (m_nameList->getSelectedItem() != -1 &&
		m_nameList->getSelectedItem() < m_nameList->getItemCount()) {
		selected = m_nameList->getItem(m_nameList->getSelectedItem()).name;
		selectSet.insert(m_nameList->getItem(m_nameList->getSelectedItem()).name);
	}
	m_nameList->clear();
	Sint32 id = 0;
	for (Matrix* m : *m_sModel->getMatrixList()) {
		m->setId(id);
		m_nameList->addItem(m->getName(), m->isVisible(), selectSet.find(m->getName()) != selectSet.end());
		if (m->getName() == selected || m_matrixEdit->getMatrix() == m) {
			m_nameList->setSelectedItem(id);
		}
		id++;
	}
}
void Model::updateVisibilityLtoM() {
	Matrix* m = 0;
	for (auto item : m_nameList->getItemList()) {
		m = m_sModel->getMatrix(item.name);
		if (m)	m->setVisible(item.visible);
	}
}
void Model::updateVisibilityMtoL() {
	updateMatrixList();
}

void Model::inputEditor(Sint8 p_guiFlags) {
	bool mouseOnGui = !(p_guiFlags & (Sint8)Component::EventFlag::MOUSEOVER);
	GLfloat _near = mouseOnGui ? 1 : 0, _far = 1;
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)
			&& m_matrixEdit->getId() != -1) {
			if (GKey::modDown(GLFW_MOD_SHIFT)) {
				//m_voxelPlaneMode = true;
				ModelMath::castRayMatrix(Camera::getPosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitMatrix(),
					_near, _far);
				ModelMath::castRayVoxel(Camera::getPosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitMatrix(),
					_near, _far,
					m_selectedVoxel,
					m_selectedSide,
					m_selectedVoxelOffset);
				m_voxelPlanePos = m_selectedVoxel;
				m_voxelPlaneSide = m_selectedSide;
			}
		}
		else {
			if (GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT)
				&& m_matrixEdit->getId() != -1) {
				ModelMath::castRayMatrix(Camera::getPosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitMatrix(),
					_near, _far);
			}
			else {
				m_voxelPlaneMode = false;
				std::vector<Matrix*> matrices = getSelectedMatrices();
				if (matrices.empty()) {
					matrices = getVisibleMatrices();
				}
				ModelMath::castRayMatrices(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), matrices, m_hoverMatrix, _near, _far);
			}
		}
	}
	else {
		ModelMath::castRayMatrices(Camera::getPosition(), Camera::getMouseDirection() * glm::vec3(4096), getVisibleMatrices(), m_hoverMatrix, _near, _far);
	}

	if (!mouseOnGui) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			if ((!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) &&
				m_hoverMatrix != m_matrixEdit->getId() &&
				(m_matrixEdit->getId() == -1 || (m_matrixEdit->getId() != -1 && m_nameList->getItem(m_matrixEdit->getId()).state != 2))) ||
				GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				hoverMatrix(m_hoverMatrix);
			}
			break;
		case Tool::ToolType::MATRIX:
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (((m_matrixEdit->getId() == -1 || m_selectedScale == 0) && static_cast<MatrixTool*>(MTool::getTool(m_tool))->getParent() == "MatrixTransform") 
					|| static_cast<MatrixTool*>(MTool::getTool(m_tool))->getParent() == "MatrixSelect") {
					if (GMouse::mouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT)) {
						// TODO
					}
					selectMatrix(m_hoverMatrix);
				}
			}
			break;
		default: break;
		}
	}

	if (!mouseOnGui && m_matrixEdit->getId() != -1 && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT) && !GMouse::mouseDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			if (m_voxelPlaneMode) {
				if (!GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					ModelMath::castRayVoxelPlane(Camera::getPosition(),
						Camera::getMouseDirection() * glm::vec3(4096),
						m_matrixEdit->getInitMatrix(),
						_near, _far,
						m_voxelPlanePos,
						m_voxelPlaneSide,
						m_selectedVoxel,
						m_selectedVoxelOffset);
					m_selectedSide = m_voxelPlaneSide;
				}
			}
			else {
				if (!GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
					ModelMath::castRayVoxel(Camera::getPosition(),
						Camera::getMouseDirection() * glm::vec3(4096),
						m_matrixEdit->getInitMatrix(),
						_near, _far, m_selectedVoxel,
						m_selectedSide,
						m_selectedVoxelOffset);
				}
			}
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

	for (Matrix* m : *m_sModel->getMatrixList()) {
		m->update();
	}

	Matrix* m = 0;
	if (!(m = getSelectedMatrix()))
		m = m_matrixEdit->getMatrix();
	if (m) {
		*m_dataString = std::string(m->getName() +
			" (" + Util::numToStringInt(m->getId()) +
			") | Pos: " + Util::numToStringFloat(m->getPos().x, 1) +
			", " + Util::numToStringFloat(m->getPos().y, 1) +
			", " + Util::numToStringFloat(m->getPos().z, 1) +
			" | Size: " + Util::numToStringInt(m->getSize().x) +
			", " + Util::numToStringInt(m->getSize().y) +
			", " + Util::numToStringInt(m->getSize().z) +
			"");
	}
	else {
		*m_dataString = "";
	}
}
void Model::renderEditor() {
	Shader::setBool("useLight", !m_wireframe);
	if (m_wireframe) {
		for (Matrix* m : *m_sModel->getMatrixList()) {
			if (m->isVisible()) {
				m->renderMatrixWireframe();
			}
		}
	}
	else {
		for (Matrix* m : *m_sModel->getMatrixList()) {
			if (m->isVisible()) {
				m->renderMatrix();
			}
		}
	}

	Shader::setBool("useLight", false);

	for (Uint16 i = 0; i < m_sModel->getMatrixList()->size(); i++) {
		if (getMatrix(i)->isVisible())
			m_sModel->getMatrix(i)->renderOutline(m_outline ? static_cast<Matrix::OutlineType>(m_nameList->getItem(i).state + 1) : Matrix::OutlineType::NONE);
	}

	if (m_grid) renderGrid();

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
	for (Matrix* m : *m_sModel->getMatrixList()) {
		m->renderShadow();
	}
}
void Model::renderGrid() {
	GLfloat w1 = 0.05f, w2 = w1 / 2.5f;
	GLfloat gc = 32; // Grid Count
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), glm::vec3(0.f, -0.025f, 0.f)));
	Shader::applyModel();
	Color& gridColor = Component::getElementColor("modelGrid");
	glm::vec4 colorVec = glm::vec4(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
	glm::vec4 colorVecInset = colorVec;
	colorVecInset.a *= 0.5f;
	Shader::setColor(glm::vec4(1.f));
	for (Sint16 i = -gc; i <= gc; i++) {
		if (i == 0) {
			MMesh::renderLine(glm::vec3(-gc, 0, 0), glm::vec3(0, 0, 0), colorVec);
			MMesh::renderLine(glm::vec3(0, 0, -gc), glm::vec3(0, 0, 0), colorVec);

			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(gc, 0, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, gc, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, gc), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else {
			if (i % 2 == 0) {
				MMesh::renderLine(glm::vec3(i, 0, -gc), glm::vec3(i, 0, gc), colorVec);
				MMesh::renderLine(glm::vec3(-gc, 0, i), glm::vec3(gc, 0, i), colorVec);
			}
			else {
				MMesh::renderLine(glm::vec3(i, 0, -gc), glm::vec3(i, 0, gc), colorVecInset);
				MMesh::renderLine(glm::vec3(-gc, 0, i), glm::vec3(gc, 0, i), colorVecInset);
			}
		}
	}
	Shader::popMatrixModel();
}

std::vector<std::string> Model::getMatrixNames() {
	return m_sModel->getMatrixNames();
}
Matrix* Model::getMatrix(Sint16 id) {
	if (id < 0 || id >= Sint16(m_sModel->getMatrixList()->size())) return 0;
	return m_sModel->getMatrix(id);
}
Matrix* Model::getMatrix(std::string p_name) {
	for (Matrix* m : *m_sModel->getMatrixList()) {
		if (m->getName() == p_name) return m;
	}
	return 0;
}
std::vector<Matrix*> Model::getVisibleMatrices() {
	std::vector<Matrix*> matrices;
	for (auto li : m_nameList->getItemList()) {
		if (li.visible) {
			matrices.push_back(getMatrix(li.name));
		}
	}
	return matrices;
}
Matrix* Model::getSelectedMatrix() {
	if (m_nameList->getSelectedItem() != -1) {
		return getMatrix(m_nameList->getSelectedItem());
	}
	return 0;
}
std::vector<Matrix*> Model::getSelectedMatrices() {
	std::vector<Matrix*> matrices;
	for (auto li : m_nameList->getItemList()) {
		if (li.state == 2) {
			matrices.push_back(getMatrix(li.name));
		}
	}
	return matrices;
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

void Model::addImageMatrix(std::string p_filename) {
	Texture* tex = MTexture::loadTexture(p_filename);
	Sint32 mid = addMatrix(p_filename.substr(p_filename.find_last_of('\\') + 1),
		Camera::getFocus() - (glm::vec3(tex->getSize().x, tex->getSize().y, 1) / 2.f),
		glm::ivec3(tex->getSize().x, tex->getSize().y, 1));
	Matrix* matrix = getMatrix(mid);
	for (Sint32 x = 0; x < tex->getSize().x; x++) {
		for (Sint32 y = 0; y < tex->getSize().y; y++) {
			Uint8 r = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 0];
			Uint8 g = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 1];
			Uint8 b = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 2];
			Uint8 a = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 3];
			if (a) {
				matrix->setVoxel(glm::ivec3(x, tex->getSize().y - y - 1, 0),
					Voxel(1, MColor::getInstance().getUnitID(Color(r / 255.f, g / 255.f, b / 255.f))));
			}
		}
	}
}

void Model::useScriptColorPalette() {
	m_scriptColor[0] = m_voxelColor->r * 255;
	m_scriptColor[1] = m_voxelColor->g * 255;
	m_scriptColor[2] = m_voxelColor->b * 255;
}
Sint32 Model::addScriptMatrix(std::string p_name, Sint32 width, Sint32 height, Sint32 depth) {
	Sint32 id = Editor::getModel()->addMatrix(p_name, glm::ivec3(), glm::ivec3(width, height, depth));
	setScriptMatrixSelected(id);
	return id;
}
Sint32 Model::getScriptMatrixByName(std::string p_name) {
	return Editor::getModel()->getMatrix(p_name)->getId();
}
void Model::setScriptMatrixPos(Sint32 p_matrix, GLfloat x, GLfloat y, GLfloat z) {
	m_scriptMatrix->setPosition(glm::vec3(x, y, z));
}
void Model::setScriptMatrixSize(Sint32 p_matrix, Sint32 w, Sint32 h, Sint32 d) {
	m_scriptMatrix->setSize(glm::ivec3(w, h, d));
}
void Model::setScriptMatrixSelected(Sint32 id) {
	Editor::getModel()->setSelectedMatrix(id);
	m_scriptMatrix = Editor::getModel()->getEditMatrix()->getMatrix();

	glm::vec3 pos = m_scriptMatrix->getPos();
	m_scriptPos[0] = pos.x;
	m_scriptPos[1] = pos.y;
	m_scriptPos[2] = pos.z;

	glm::ivec3 size = m_scriptMatrix->getSize();
	m_scriptSize[0] = size.x;
	m_scriptSize[1] = size.y;
	m_scriptSize[2] = size.z;
}
void Model::getScriptColor(Sint32 x, Sint32 y, Sint32 z) {
	Color& voxelColor = MColor::getInstance().getUnit(m_scriptMatrix->getVoxel(glm::ivec3(x, y, z)).color);
	m_scriptColor[0] = voxelColor.r * 255;
	m_scriptColor[1] = voxelColor.g * 255;
	m_scriptColor[2] = voxelColor.b * 255;
}
void Model::setScriptVoxel(Sint32 x, Sint32 y, Sint32 z, Sint32 p_interAction) {
	if (p_interAction) {
		m_scriptMatrix->setVoxel(glm::ivec3(x, y, z),
			Voxel(p_interAction, MColor::getInstance().getUnitID(Color(m_scriptColor[0] / 255.f, m_scriptColor[1] / 255.f, m_scriptColor[2] / 255.f))));
	}
	else {
		m_scriptMatrix->setVoxel(glm::ivec3(x, y, z),
			Voxel(p_interAction, 0));
	}
}
void Model::loadScriptFile(std::string p_scriptFile) {
	if (Editor::isModel() && Editor::getModel()->getEditMatrix()->getId() != -1) {
		setScriptMatrixSelected(Editor::getModel()->getEditMatrix()->getId());
		GLua::loadScriptFile(p_scriptFile);
		Editor::getModel()->getEditMatrix()->reset(true);
	} else {
		if (!Editor::isModel()) {
			Logger::logError("Model is not currently loaded");
		} else {
			Logger::logError("No selected matrix found!");
		}
	}
}
void Model::loadScriptString(std::string p_script) {
	if (Editor::isModel() && Editor::getModel()->getSelectedMatrix()) {
		setScriptMatrixSelected(Editor::getModel()->getEditMatrix()->getId());
		GLua::loadScriptString(p_script);
		Editor::getModel()->getEditMatrix()->reset(true);
	} else {
		if (!Editor::isModel()) {
			Logger::logError("Model is not currently loaded");
		} else {
			Logger::logError("No selected matrix found!");
		}
	}
}

bool Model::exitSave() {
	char documents[MAX_PATH] = "\0";
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	int success = _mkdir(documents);

	char filename[MAX_PATH] = "\0";
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Model (*.csm)\0*.csm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save As";

	if (!GetSaveFileName(&ofn)) return false;
	LFormat::save(filename, m_sModel->getMatrixList());
	return true;
}
void Model::autosave() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Chronovox Studio\\autosave.csm");
	CsmFormat::save(documents, m_sModel->getMatrixList());
}
bool Model::autoload() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Chronovox Studio\\autosave.csm");
	return loadOpen(documents);
}
void Model::add() {
	char documents[MAX_PATH] = "\0";
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	int success = _mkdir(documents);

	char filename[MAX_PATH] = "\0";
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Model (*.csm, *.nvm, *.qb)\0"
		"*.csm;*.nvm;*.qb*\0"
		//"Animation (*.nva)\0"
		//"*.nva\0"
		"Any File\0"
		"*.*\0\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Add Model";
	if (!GetOpenFileName(&ofn)) return;
	loadAdd(filename);
	updateMatrixList();
}
void Model::newModel() {
	if (m_sModel->getMatrixList()->size() > 0) {
		save();
		for (Matrix* m : *m_sModel->getMatrixList()) {
			delete m;
		}
		m_sModel->getMatrixList()->clear();
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearMatrix(false);
		m_nameList->clear();
	}
	addMatrix("Matrix", { -5, 0, -5 }, { 10, 10, 10 });
}
bool Model::save() {
	if (m_directory != "") {
		LFormat::save(getPath(), m_sModel->getMatrixList());
		m_matrixEdit->setChanged(false);
		return true;
	}
	else {
		return saveAs();
	}
	return false;
}
bool Model::saveAs() {
	char documents[MAX_PATH] = "\0";
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	int success = _mkdir(documents);

	char filepath[MAX_PATH] = "\0";
	char filename[MAX_PATH] = "\0";
	OPENFILENAME ofn;

	ZeroMemory(&filepath, sizeof(filepath));
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "CSM (*.csm)\0*.csm*\0Any File\0*.*\0";
	ofn.lpstrFile = filepath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = MAX_PATH;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.lpstrTitle = "Save As";
	ofn.Flags = ofn.Flags | OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;
	ofn.lpstrDefExt = "csm\0";
	if (!GetSaveFileName(&ofn)) return false;

	LFormat::save(filepath, m_sModel->getMatrixList());
	std::string fp = filepath;
	setDirectory(fp.substr(0, fp.find_last_of('\\') + 1));
	setName(filename);
	m_matrixEdit->setChanged(false);
	return true;
}
bool Model::loadOpen(std::string p_fileName) {
	LFormat::ImportType _formatType = LFormat::valid(p_fileName);
	if (_formatType != LFormat::ImportType::NONE) {
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearMatrix(false);
		m_nameList->clear();
		m_sModel->getMatrixList()->clear();
		setPath(p_fileName);
		GFormat::setLoadFunction([&]() { 
			updateMatrixList();
			m_matrixEdit->setChanged(false);
			setBuilding(false);
			});
		LFormat::load(p_fileName, m_sModel->getMatrixList(), _formatType);
		return true;
	}
	return false;
}
bool Model::loadAdd(std::string p_fileName) {
	LFormat::ImportType _formatType = LFormat::valid(p_fileName);
	if (_formatType != LFormat::ImportType::NONE) {
		setBuilding(true);
		GFormat::setLoadFunction([&]() {
			updateMatrixList();
			m_matrixEdit->setChanged(true);
			setBuilding(false);
			});
		LFormat::load(p_fileName, m_sModel->getMatrixList(), _formatType);
		return true;
	}
	return false;
}
bool Model::exportAs(LFormat::ExportType p_exportType) {
	if (p_exportType == LFormat::ExportType::NONE) {
		return false;
	}
	char documents[MAX_PATH] = "\0";
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	int success = _mkdir(documents);

	char filepath[MAX_PATH] = "\0";
	char filename[MAX_PATH] = "\0";
	OPENFILENAME ofn;

	ZeroMemory(&filepath, sizeof(filepath));
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrTitle = "Export";
	switch (p_exportType) {
	case LFormat::ExportType::STL:
		ofn.lpstrFilter = "STL (*.stl)\0*.stl*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "stl\0";
		break;
	case LFormat::ExportType::OBJ:
		ofn.lpstrFilter = "Wavefront (*.obj)\0*.obj*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "obj\0";
		break;
	case LFormat::ExportType::PLY:
		ofn.lpstrFilter = "Stanford (*.ply)\0*.ply*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "ply\0";
		break;
	case LFormat::ExportType::FBX:
		ofn.lpstrFilter = "FBX (*.fbx)\0*.fbx*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "fbx\0";
		break;
	case LFormat::ExportType::COLLADA:
		ofn.lpstrFilter = "COLLADA (*.dae)\0*.dae*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "dae\0";
		break;
	case LFormat::ExportType::TDS:
		ofn.lpstrFilter = "3DS (*.3ds)\0*.3ds*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "3ds\0";
		break;
	case LFormat::ExportType::IGES:
		ofn.lpstrFilter = "IGES (*.igs)\0*.igs*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "igs\0";
		break;
	case LFormat::ExportType::STEP:
		ofn.lpstrFilter = "STEP (*.stp)\0*.stp*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "stp\0";
		break;
	case LFormat::ExportType::X3D:
		ofn.lpstrFilter = "X3D (*.wrl)\0*.wrl*\0Any File\0*.*\0";
		ofn.lpstrDefExt = "wrl\0";
		break;
	}
	ofn.lpstrFile = filepath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = MAX_PATH;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.Flags = ofn.Flags | OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;
	if (!GetSaveFileName(&ofn)) return false;

	LFormat::exportFile(filepath, m_sModel->getMatrixList(), p_exportType);
	return true;
}

void Model::fileNew() {
	m_matrixEdit->clearMatrix();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		m_matrixEdit->addCommand(new MDeleteCommand(m_sModel->getMatrix(i), *m_sModel->getMatrixList(), i, m_nameList));
		m_nameList->removeItem(i);
		m_sModel->getMatrixList()->erase(m_sModel->getMatrixList()->begin() + i);
	}
	m_matrixEdit->setCommandChaining(false);
	updateMatrixList();
}
void Model::fileAdd() {
	add();
}
bool Model::fileSave() {
	return save();
}
bool Model::fileSaveAs() {
	return saveAs();
}
void Model::fileExit() {

}
void Model::fileExport(LFormat::ExportType p_exportType) {
	switch (p_exportType) {
	case LFormat::ExportType::OBJ:
		Gui::openDialog("ObjExport.lua");
		Gui::topDialog()->addFunction([&]() {
			glm::vec3 scale = glm::vec3(
				Gui::topDialog()->findComponent("SCALE0")->getValue(),
				Gui::topDialog()->findComponent("SCALE1")->getValue(),
				Gui::topDialog()->findComponent("SCALE2")->getValue());
			glm::vec3 offset = glm::vec3(
				Gui::topDialog()->findComponent("OFFSET0")->getValue(),
				Gui::topDialog()->findComponent("OFFSET1")->getValue(),
				Gui::topDialog()->findComponent("OFFSET2")->getValue());
			GLfloat texelSize = Gui::topDialog()->findComponent("TEXELSIZE")->getValue();
			std::string subfolder = Gui::topDialog()->findComponent("TEXSUBFOLDER")->getTitle();
			ObjFormat::init(scale, offset, texelSize, subfolder);
			exportAs(LFormat::ExportType::OBJ);
			});
		break;
	case LFormat::ExportType::PLY:
		Gui::openDialog("PlyExport.lua");
		Gui::topDialog()->addFunction([&]() {
			glm::vec3 scale = glm::vec3(
				Gui::topDialog()->findComponent("SCALE0")->getValue(),
				Gui::topDialog()->findComponent("SCALE1")->getValue(),
				Gui::topDialog()->findComponent("SCALE2")->getValue());
			glm::vec3 offset = glm::vec3(
				Gui::topDialog()->findComponent("OFFSET0")->getValue(),
				Gui::topDialog()->findComponent("OFFSET1")->getValue(),
				Gui::topDialog()->findComponent("OFFSET2")->getValue());
			PlyFormat::init(scale, offset);
			exportAs(LFormat::ExportType::PLY);
			});
		break;
	case LFormat::ExportType::STL:
		Gui::openDialog("StlExport.lua");
		Gui::topDialog()->addFunction([&]() {
			glm::vec3 scale = glm::vec3(
				Gui::topDialog()->findComponent("SCALE0")->getValue(),
				Gui::topDialog()->findComponent("SCALE1")->getValue(),
				Gui::topDialog()->findComponent("SCALE2")->getValue());
			glm::vec3 offset = glm::vec3(
				Gui::topDialog()->findComponent("OFFSET0")->getValue(),
				Gui::topDialog()->findComponent("OFFSET1")->getValue(),
				Gui::topDialog()->findComponent("OFFSET2")->getValue());
			StlFormat::init(scale, offset);
			exportAs(LFormat::ExportType::STL);
			});
		break;
	default: break;
	}
}

void Model::editMergeMatrix() {
	std::vector<Matrix*> matrices = getSelectedMatrices();
	if (matrices.size() > 1) {
		m_matrixEdit->setCommandChaining(true);
		m_matrixEdit->reset(false);
		Matrix* selected = new Matrix(*m_sModel->getMatrix(matrices[0]->getId()));
		for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
			if (m_nameList->getItem(i).state == 2 && i != matrices[0]->getId()) {
				m_sModel->getMatrix(matrices[0]->getId())->merge(*m_sModel->getMatrix(i));
			}
		}
		m_matrixEdit->addCommand(new MResizeCommand(m_sModel->getMatrix(matrices[0]->getId()), selected));
		for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
			if (m_nameList->getItem(i).state == 2 && i != matrices[0]->getId()) {
				m_matrixEdit->addCommand(new MDeleteCommand(m_sModel->getMatrix(i), *m_sModel->getMatrixList(), i, m_nameList));
				m_nameList->removeItem(i);
				m_sModel->getMatrixList()->erase(m_sModel->getMatrixList()->begin() + i);
			}
		}
		m_matrixEdit->setCommandChaining(false);
		m_matrixEdit->reset(false);
		updateMatrixList();
		selectMatrix(matrices[0]->getId());
		Logger::logNormal("Merged matrices");
	}
	else {
		Logger::logNormal("Must select more than one matrices to merge");
	}
}
void Model::editNewMatrix() {
	/*
	NewModelDialog::getInstance().getDialog()->setOptionFunc("Create", [&]() {
		CDialog* nmd = NewModelDialog::getInstance().getDialog();
		if (nmd->findComponent("WIDTH")->getValue() > 0
			&& nmd->findComponent("HEIGHT")->getValue() > 0
			&& nmd->findComponent("DEPTH")->getValue() > 0) {
			glm::vec3 pos = glm::vec3(nmd->findComponent("WIDTH")->getValue() / -2,
				nmd->findComponent("HEIGHT")->getValue() / -2,
				nmd->findComponent("DEPTH")->getValue() / -2) + Camera::getFocus();
			this->addMatrix(nmd->findComponent("MATRIXNAME")->getTitle() == "" ? "Matrix" : nmd->findComponent("MATRIXNAME")->getTitle(), pos,
				glm::ivec3(nmd->findComponent("WIDTH")->getValue(),
					nmd->findComponent("HEIGHT")->getValue(),
					nmd->findComponent("DEPTH")->getValue()));
		}
		nmd->setActive(false);
	});
	*/
	//Gui::openDialog(NewModelDialog::getInstance().getDialog());
	Gui::openDialog("NewMatrix.lua");
	Gui::topDialog()->addFunction([&]() {
		std::string name = Gui::topDialog()->findComponent("MATRIXNAME")->getTitle();
		GLfloat width	 = Gui::topDialog()->findComponent("SIZE0")->getValue();
		GLfloat height	 = Gui::topDialog()->findComponent("SIZE1")->getValue();
		GLfloat depth	 = Gui::topDialog()->findComponent("SIZE2")->getValue();
		addMatrix(name,
			glm::vec3(width / -2, height / -2, depth / -2),
			glm::ivec3(width, height, depth));
		});
}
void Model::editMatrixProperties() {
	Matrix* m = m_matrixEdit->getMatrix();
	if (!m) return;

	Gui::openDialog("MatrixProperties.lua");
	Gui::topDialog()->findComponent("NAME")		->setTitle(m->getName());
	Gui::topDialog()->findComponent("PARENT")	->setTitle(m->getParent());
	Gui::topDialog()->findComponent("POS0")		->setValue(m->getPos().x);
	Gui::topDialog()->findComponent("POS1")		->setValue(m->getPos().y);
	Gui::topDialog()->findComponent("POS2")		->setValue(m->getPos().z);
	Gui::topDialog()->findComponent("SIZE0")	->setValue(m->getSize().x);
	Gui::topDialog()->findComponent("SIZE1")	->setValue(m->getSize().y);
	Gui::topDialog()->findComponent("SIZE2")	->setValue(m->getSize().z);
	Gui::topDialog()->addFunction([&]() {
		m_matrixEdit->reset(true);
		renameMatrix(m_matrixEdit->getMatrix()->getId(), Gui::topDialog()->findComponent("NAME")->getTitle());
		m_matrixEdit->getMatrix()->setParent(Gui::topDialog()->findComponent("PARENT")->getTitle());
		m_matrixEdit->getMatrix()->setPosition(glm::vec3(	Gui::topDialog()->findComponent("POS0")->getValue(),
															Gui::topDialog()->findComponent("POS1")->getValue(),
															Gui::topDialog()->findComponent("POS2")->getValue()));
		m_matrixEdit->getMatrix()->setSize(glm::ivec3(	Gui::topDialog()->findComponent("SIZE0")->getValue(),
														Gui::topDialog()->findComponent("SIZE1")->getValue(),
														Gui::topDialog()->findComponent("SIZE2")->getValue()));
		getEditMatrix()->addCommand(new MResizeCommand(getEditMatrix()->getMatrix(), getEditMatrix()->getInitMatrix()));
		getEditMatrix()->reset(false);
		});
}
void Model::editCopy() {
	copyMatrix();
}
void Model::editCut() {
	copyMatrix();
	deleteSelectedMatrices();
}
void Model::editPaste() {
	pasteMatrix();
}
void Model::editUndo() {
	undo();
	//updateMatrixList();
}
void Model::editRedo() {
	redo();
	//updateMatrixList();
}

void Model::matrixSize() {
	Matrix* m = m_matrixEdit->getMatrix();
	if (!m) return;

	Gui::openDialog("ResizeMatrix.lua");
	Gui::topDialog()->findComponent("SIZE0")->setValue(m->getSize().x);
	Gui::topDialog()->findComponent("SIZE1")->setValue(m->getSize().y);
	Gui::topDialog()->findComponent("SIZE2")->setValue(m->getSize().z);
	Gui::topDialog()->addFunction([&]() { 
		m_matrixEdit->reset(true);
		m_matrixEdit->getMatrix()->scale(glm::vec3(	Gui::topDialog()->findComponent("SIZE0")->getValue() / m_matrixEdit->getMatrix()->getSize().x,
													Gui::topDialog()->findComponent("SIZE1")->getValue() / m_matrixEdit->getMatrix()->getSize().y,
													Gui::topDialog()->findComponent("SIZE2")->getValue() / m_matrixEdit->getMatrix()->getSize().z),
			glm::vec3());
		m_matrixEdit->addCommand(new MResizeCommand(m_matrixEdit->getMatrix(), m_matrixEdit->getInitMatrix()));
		m_matrixEdit->reset(false);
		});
}

void Model::generateCustom() {
	Gui::openDialog("Generate.lua");
	Gui::topDialog()->addFunction([&]() {
		m_matrixEdit->reset(true);
		loadScriptString(Gui::topDialog()->findComponent("SCRIPT")->getTitle());
		m_matrixEdit->reset(false);
		});
}
