#include "engine\editor\map\Map.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\Utilities.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\map\menu\MapOverlay.h"
#include "engine\gfx\gui\component\Component.h"
#include "engine\utils\global\GLua.h"
#include "engine\editor\GPreferences.h"
#include "engine\editor\GRecents.h"

#include <direct.h>
#include <shlobj.h>
#include <iostream>
#include <set>

Sint32 Map::m_tool;

glm::ivec3 Map::m_selectedVoxel;
glm::ivec3 Map::m_selectedVoxelOffset;
Sint8 Map::m_selectedSide;
Color* Map::m_voxelColor;
GLfloat Map::m_scriptPos[3];
Sint32 Map::m_scriptSize[3];
Sint32 Map::m_scriptColor[3];
Chunk* Map::m_scriptChunk;

Map::Map()
	: TEMode() {
	m_sMap = new SimpleMap();
	m_matrixEdit = new EditChunk();
}
Map::~Map() {
	delete m_sMap;
	delete m_matrixEdit;
}
void Map::init() {
	m_name = "New Map";
	m_directory = "";
	m_matrixCopy = 0;
	m_hoverChunk = -1;
	m_grid = true;
	m_outline = true;
	m_wireframe = false;
	m_nameList = (CList*)MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_MATRICES\\LIST_MATRICES");
	m_colorOverlay = (ColorOverlay*)MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_COLOR\\OVERLAY_COLOR");
	m_colorOverlay->setColorRGB(0, 255, 128);
	m_voxelColor = &m_colorOverlay->getColor();
	newMap();
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

	GLua::getGlobal().addFunction("addChunk", &addScriptChunk);
	GLua::getGlobal().addFunction("getChunkByName", &getScriptChunkByName);
	GLua::getGlobal().addFunction("setChunkPos", &setScriptChunkPos);
	GLua::getGlobal().addFunction("setChunkSize", &setScriptChunkSize);
	GLua::getGlobal().addFunction("setSelectedChunk", &setScriptChunkSelected);

	GLua::getGlobal().addFunction("useColor", &useScriptColorPalette);
	GLua::getGlobal().addFunction("getColor", &getScriptColor);
	GLua::getGlobal().addFunction("setVoxel", &setScriptVoxel);
}
void Map::activate() {
	Tool::init(m_matrixEdit);
	VoxelTool::init(&m_selectedVoxel, &m_selectedVoxelOffset, &m_selectedSide, m_voxelColor);
	ChunkTool::init(m_sMap->getChunkList(), &m_scalePos, &m_selectedScale);
}

bool Map::hasChanged() {
	return m_matrixEdit->hasChanged();
}
void Map::setChanged(bool p_changed) {
	m_matrixEdit->setChanged(p_changed);
}

void Map::setDataString(std::string* p_dataString) {
	m_dataString = p_dataString;
}
void Map::setTool(Sint32 p_tool) {
	Logger::logQuiet("test1");
	if (m_tool == p_tool) {
		static_cast<CButtonRadio*>(MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN"))->incrementSelected();
	}
	else {
		m_tool = p_tool;
		MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN")->setSelectedItem(p_tool);
		Tool* tool = MTool::getTool(m_tool);
	}
}
void Map::setTool(std::string p_toolName) {
	Logger::logQuiet("test2");
	Sint32 select = -1;
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
				select = i;
				break;
			}
		}
		if (select != -1) {
			if (m_tool == select) {
				static_cast<CButtonRadio*>(MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN"))->incrementSelected();
			}
			m_tool = select;
			MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN")->setSelectedItem(m_tool);
			MTool::getTool(m_tool)->enable();
		}
	}
}
void Map::updateTool() {
	m_tool = MTool::getToolId(static_cast<CButtonRadio*>(MapOverlay::getContainer()->findComponent("GUI_WORKSPACE\\GUI_TOOLBAR\\TOOLBAR_MAIN"))->getSelectedRadio());
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
	} else if (MTool::getTool(m_tool)->getType() == Tool::ToolType::MATRIX) {
		hoverChunk(-1);
	}
}

void Map::focus() {
	if (m_matrixEdit->getId() != -1) {
		if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
			if (m_matrixEdit->getChunk()->containsPoint(m_selectedVoxel)) {
				Camera::setPosition(m_matrixEdit->getPos() + glm::vec3(m_selectedVoxel) + glm::vec3(0.5f, 0.5f, 0.5f));
			}
			else {
				Camera::setPosition(m_matrixEdit->getChunk()->getCenter());
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

void Map::fixSelectedChunk() {
	for (Sint32 i = 0; i < (Sint32)m_nameList->getItemList().size(); i++) {
		auto li = m_nameList->getItemList().at(i);
		getChunk(li.name)->setId(i);
	}
	if (m_nameList->getSelectedItem() == -1) {
		m_matrixEdit->clearChunk();
	}
	else {
		m_matrixEdit->setChunk(getChunk(m_nameList->getSelectedItem()), m_nameList->getSelectedItem());
	}
}

void Map::undo() {
	m_matrixEdit->undo();
	updateChunkList();
	fixSelectedChunk();
}
void Map::redo() {
	m_matrixEdit->redo();
	updateChunkList();
	fixSelectedChunk();
}

void Map::copyChunk() {
	if (m_matrixCopy) delete m_matrixCopy;
	if (m_matrixEdit->getId() != -1)
		m_matrixCopy = new Chunk(*m_sMap->getChunk(m_matrixEdit->getId()));
}
void Map::pasteChunk() {
	if (m_matrixCopy) {
		m_matrixEdit->setCommandListOpen(false);
		addChunk(m_matrixCopy->getName() + " - Copy", Camera::getPosition() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)), m_matrixCopy->getSize());
		Chunk* _matrix = getChunk(Sint16(getChunkNames().size()) - 1);
		_matrix->setPosition(Camera::getFocus() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)));
		MNewCommand* _cmd = new MNewCommand(_matrix, *m_sMap->getChunkList(), Uint16(m_sMap->getChunkList()->size()) - 1, m_nameList);
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
		m_matrixEdit->addCommand(new MNewCommand(m_sMap->getChunk(Sint16(m_sMap->getChunkList()->size()) - 1), *m_sMap->getChunkList(), Uint16(m_sMap->getChunkList()->size()) - 1, m_nameList));
	}
}

void Map::setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel) {
	m_sMap->setVoxel(p_matrix, p_pos, p_voxel);
}
Voxel Map::getVoxel(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_sMap->getVoxel(p_matrix, p_pos);
}
Uint16 Map::getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos) {
	return m_sMap->getVoxelId(p_matrix, p_pos);
}

void Map::resize(Uint16 p_matrixId, glm::ivec3 p_offset, glm::ivec3 p_size) {
	if (p_size.x <= p_offset.x || p_size.y <= p_offset.y || p_size.z <= p_offset.z) return;
	Sint32 id = m_matrixEdit->getId();

	m_matrixEdit->setChunk(m_sMap->getChunk(p_matrixId), p_matrixId);
	m_matrixEdit->getChunk()->setSize(p_size);
	m_matrixEdit->getChunk()->shiftVoxels(-p_offset);
	m_matrixEdit->getChunk()->setSize(p_size - p_offset);
	m_matrixEdit->getChunk()->addPosition(p_offset);

	if (id != -1)
		m_matrixEdit->setChunk(m_sMap->getChunk(id), id);
	else
		m_matrixEdit->clearChunk();
}
void Map::shiftChunk(glm::ivec3 p_direction) {
	if (m_hoverChunk != -1) {
		m_sMap->getChunk(m_hoverChunk)->shiftVoxels(p_direction);
	}
}
void Map::flipChunk(Sint8 p_axesFlags) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Chunk* m : getSelectedMatrices()) {
		m_matrixEdit->setChunk(m, m->getId());
		m->flip(p_axesFlags, center);
		m_matrixEdit->clearChunk();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedChunk();
}
void Map::rotateChunk(Sint8 p_axesFlags) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Chunk* m : getSelectedMatrices()) {
		m_matrixEdit->setChunk(m, m->getId());
		m->rotate(p_axesFlags, center);
		m_matrixEdit->clearChunk();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedChunk();
}
void Map::scaleChunk(glm::vec3 p_scale) {
	glm::vec3 center = getSelectedMatricesCenter();
	m_matrixEdit->setCommandChaining(true);
	for (Chunk* m : getSelectedMatrices()) {
		m_matrixEdit->setChunk(m, m->getId());
		m->scale(p_scale, center);
		m_matrixEdit->clearChunk();
	}
	m_matrixEdit->setCommandChaining(false);
	fixSelectedChunk();
}
Sint32 Map::addChunk(std::string p_name, glm::vec3 p_pos, glm::ivec3 p_size) {
	if (p_name == "") p_name = "Chunk";
	bool validName;
	Uint16 i = 0;
	std::string _name = p_name;
	do {
		validName = true;
		for (Chunk *m : *m_sMap->getChunkList()) {
			if (m->getName() == _name) {
				validName = false;
				_name = p_name + Util::numToStringInt(i);
				i++;
				break;
			}
		}
	} while (!validName);
	Sint16 id = Sint16(m_sMap->getChunkList()->size());
	Chunk* matrix = new Chunk(id, _name, "", p_pos, p_size);
	m_sMap->getChunkList()->push_back(matrix);
	MNewCommand* _cmd = new MNewCommand(m_sMap->getChunk(Sint16(m_sMap->getChunkList()->size()) - 1), *m_sMap->getChunkList(), Uint16(m_sMap->getChunkList()->size()) - 1, m_nameList);
	m_matrixEdit->addCommand(_cmd);
	glm::vec3 _pos = m_pos = { 1000, 1000, 1000 };
	m_size = { -1000, -1000, -1000 };
	for (Chunk *m : *m_sMap->getChunkList()) {
		_pos = glm::vec3(std::fminf(m_pos.x, m->getPos().x),
			std::fminf(m_pos.y, m->getPos().y),
			std::fminf(m_pos.z, m->getPos().z));
		m_size = glm::vec3(std::fmaxf(m_pos.x + m_size.x, m->getPos().x + m->getSize().x) - _pos.x,
			std::fmaxf(m_pos.y + m_size.y, m->getPos().y + m->getSize().y) - _pos.y,
			std::fmaxf(m_pos.z + m_size.z, m->getPos().z + m->getSize().z) - _pos.z);
		m_pos = _pos;
	}
	updateChunkList();
	return static_cast<Sint32>(m_sMap->getChunkList()->size()) - 1;
}
void Map::renameChunk(Uint16 id, std::string p_name) {
	if (p_name == "") p_name = "Chunk";
	for (Chunk* m : *m_sMap->getChunkList()) {
		if (m->getId() != id && m->getName() == p_name) {
			Logger::logQuiet("Could not rename matrix: Name already taken");
			return;
		}
	}
	m_sMap->getChunk(id)->setName(p_name);
	m_nameList->getItem(id).name = p_name;
	updateChunkList();
}
void Map::deleteSelectedMatrices() {
	m_matrixEdit->clearChunk();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		if (m_nameList->getItem(i).state == 2) {
			m_matrixEdit->addCommand(new MDeleteCommand(m_sMap->getChunk(i), *m_sMap->getChunkList(), i, m_nameList));
			m_nameList->removeItem(i);
			delete m_sMap->getChunkList()->at(i);
			m_sMap->getChunkList()->erase(m_sMap->getChunkList()->begin() + i);
		}
	}
	m_matrixEdit->setCommandChaining(false);
	updateChunkList();
}
void Map::moveChunk(bool p_up) {
	// Reversed because it is rendered opposite
	std::vector<CList::ListItem> fromOrder = m_nameList->getItemList();
	if (!p_up) {
		for (Sint32 i = m_nameList->getItemCount() - 2; i >= 0; i--) {
			if (m_nameList->getItem(i).state != 2) continue;
			m_sMap->getChunkList()->insert(m_sMap->getChunkList()->begin() + i + 2, m_sMap->getChunk(i));
			m_sMap->getChunkList()->erase(m_sMap->getChunkList()->begin() + i);
			m_nameList->insertItem(i + 2, m_nameList->getItem(i).name);
			m_nameList->getItem(i + 2).state = 2;
			m_nameList->removeItem(i);
		}
		if (m_nameList->getSelectedItem() == 0) {
			m_nameList->setSelectedItem(m_nameList->getItemCount() - 1);
		} else {
			m_nameList->setSelectedItem(m_nameList->getSelectedItem() - 1);
		}
	}
	else {
		for (Sint32 i = 1; i < m_nameList->getItemCount(); i++) {
			if (m_nameList->getItem(i).state != 2) continue;
			m_sMap->getChunkList()->insert(m_sMap->getChunkList()->begin() + i - 1, m_sMap->getChunk(i));
			m_sMap->getChunkList()->erase(m_sMap->getChunkList()->begin() + i + 1);
			m_nameList->insertItem(i - 1, m_nameList->getItem(i).name);
			m_nameList->getItem(i - 1).state = 2;
			m_nameList->removeItem(i + 1);
		}
		if (m_nameList->getSelectedItem() == m_nameList->getItemCount() - 1) {
			m_nameList->setSelectedItem(0);
		}
		else {
			m_nameList->setSelectedItem(m_nameList->getSelectedItem() + 1);
		}
	}
	Sint32 id = 0;
	for (Chunk* m : *m_sMap->getChunkList()) {
		m->setId(id);
		id++;
	}
	m_matrixEdit->addCommand(new MReorderCommand(m_sMap->getChunkList(), fromOrder, m_nameList));
}
void Map::hoverChunk(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1) {
			if (m_nameList->getItem(m_matrixEdit->getId()).state != 2) {
				m_nameList->getItem(m_matrixEdit->getId()).state = 0;
				m_matrixEdit->clearChunk();
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
		m_matrixEdit->setChunk(m_sMap->getChunk(id), id);
	}
}
void Map::setSelectedChunk(Sint16 id) {
	m_matrixEdit->clearChunk();
	if (id != -1) {
		m_matrixEdit->setChunk(getChunk(id), id);
	}
}
void Map::selectChunk(Sint16 id) {
	if (id == -1) {
		if (m_matrixEdit->getId() != -1) {
			if (m_nameList->getItem(m_matrixEdit->getId()).state == 2) {
				m_nameList->selectItem(m_matrixEdit->getId());
			}
			m_matrixEdit->clearChunk();
		}
	}
	else {
		if (m_nameList->getItem(id).state != 2) {
			m_nameList->selectItem(id);
		}
		if (m_nameList->getItem(id).state == 2) {
			m_matrixEdit->setChunk(m_sMap->getChunk(id), id);
		}
		else {
			m_matrixEdit->clearChunk();
			Sint32 i = 0;
			for (auto li : m_nameList->getItemList()) {
				if (li.state == 2) {
					m_nameList->setSelectedItem(i);
					m_matrixEdit->setChunk(m_sMap->getChunk(i), i);
					break;
				}
				i++;
			}
		}
	}
}
void Map::selectAll() {
	for (size_t i = 0; i < m_nameList->getItemList().size(); i++) {
		m_nameList->getItem((Sint16)i).state = 2;
	}
	if (!m_sMap->getChunkList()->empty()) m_matrixEdit->setChunk(getChunk(0), 0);
	updateChunkList();
}
void Map::setSelectedVisibility(bool p_visible) {
	for (Chunk* m : getSelectedMatrices()) {
		m->setVisible(p_visible);
	}
	updateVisibilityMtoL();
}

void Map::updateChunkList() {
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
	for (Chunk* m : *m_sMap->getChunkList()) {
		m->setId(id);
		m_nameList->addItem(m->getName(), m->isVisible(), selectSet.find(m->getName()) != selectSet.end());
		if (m->getName() == selected || m_matrixEdit->getChunk() == m) {
			m_nameList->setSelectedItem(id);
		}
		id++;
	}
}
void Map::updateVisibilityLtoM() {
	Chunk* m = 0;
	for (auto item : m_nameList->getItemList()) {
		m = m_sMap->getChunk(item.name);
		if (m)	m->setVisible(item.visible);
	}
}
void Map::updateVisibilityMtoL() {
	updateChunkList();
}

void Map::inputEditor(Sint8 p_guiFlags) {
	bool mouseOnGui = !(p_guiFlags & (Sint8)Component::EventFlag::MOUSEOVER);
	GLfloat _near = mouseOnGui ? 1 : 0, _far = 1;
	if (MTool::getTool(m_tool)->getType() == Tool::ToolType::VOXEL) {
		if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)
			&& m_matrixEdit->getId() != -1) {
			if (GKey::modDown(GLFW_MOD_SHIFT)) {
				//m_voxelPlaneMode = true;
				MapMath::castRayChunk(Camera::getMousePosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitChunk(),
					_near, _far);
				MapMath::castRayVoxel(Camera::getMousePosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitChunk(),
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
				MapMath::castRayChunk(Camera::getMousePosition(),
					Camera::getMouseDirection() * glm::vec3(4096),
					m_matrixEdit->getInitChunk(),
					_near, _far);
			}
			else {
				m_voxelPlaneMode = false;
				std::vector<Chunk*> matrices = getSelectedMatrices();
				if (matrices.empty()) {
					matrices = getVisibleMatrices();
				}
				MapMath::castRayMatrices(Camera::getMousePosition(), Camera::getMouseDirection() * glm::vec3(4096), matrices, m_hoverChunk, _near, _far);
			}
		}
	}
	else {
		MapMath::castRayMatrices(Camera::getMousePosition(), Camera::getMouseDirection() * glm::vec3(4096), getVisibleMatrices(), m_hoverChunk, _near, _far);
	}

	if (!mouseOnGui) {
		switch (MTool::getTool(m_tool)->getType()) {
		case Tool::ToolType::VOXEL:
			if ((!GMouse::mouseDown(GLFW_MOUSE_BUTTON_LEFT) &&
				m_hoverChunk != m_matrixEdit->getId() &&
				(m_matrixEdit->getId() == -1 || (m_matrixEdit->getId() != -1 && m_nameList->getItem(m_matrixEdit->getId()).state != 2))) ||
				GMouse::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
				hoverChunk(m_hoverChunk);
			}
			break;
		case Tool::ToolType::MATRIX:
			if (GMouse::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
				if (((m_matrixEdit->getId() == -1 || m_selectedScale == 0) && static_cast<ChunkTool*>(MTool::getTool(m_tool))->getParent() == "ChunkTransform") 
					|| static_cast<ChunkTool*>(MTool::getTool(m_tool))->getParent() == "ChunkSelect") {
					if (GMouse::mouseDoubleClicked(GLFW_MOUSE_BUTTON_LEFT)) {
						// TODO
					}
					selectChunk(m_hoverChunk);
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
					MapMath::castRayVoxelPlane(Camera::getMousePosition(),
						Camera::getMouseDirection() * glm::vec3(4096),
						m_matrixEdit->getInitChunk(),
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
					MapMath::castRayVoxel(Camera::getMousePosition(),
						Camera::getMouseDirection() * glm::vec3(4096),
						m_matrixEdit->getInitChunk(),
						_near, _far, m_selectedVoxel,
						m_selectedSide,
						m_selectedVoxelOffset);
				}
			}
			static_cast<VoxelTool*>(MTool::getTool(m_tool))->input();
			break;
		case Tool::ToolType::MATRIX:
			MapMath::castRayScale(Camera::getMousePosition(), Camera::getMouseDirection() * glm::vec3(4096), m_matrixEdit->getChunk(), _near, _far, m_scalePos, m_selectedScale);
			static_cast<ChunkTool*>(MTool::getTool(m_tool))->input();
			break;
		}
	}
	else {
		m_selectedVoxel = m_selectedVoxelOffset = { -1, -1, -1 };
	}
}
void Map::updateEditor(GLfloat p_deltaUpdate) {
	switch (MTool::getTool(m_tool)->getType()) {
	case Tool::ToolType::VOXEL:
		static_cast<VoxelTool*>(MTool::getTool(m_tool))->update();
		break;
	case Tool::ToolType::MATRIX:
		static_cast<ChunkTool*>(MTool::getTool(m_tool))->update();
		break;
	default: break;
	}

	for (Chunk* m : *m_sMap->getChunkList()) {
		m->update();
	}

	Chunk* m = 0;
	if (!(m = getSelectedChunk()))
		m = m_matrixEdit->getChunk();
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
void Map::renderEditor() {
	Shader::setBool("useLight", !m_wireframe);
	if (m_wireframe) {
		for (Chunk* m : *m_sMap->getChunkList()) {
			if (m->isVisible()) {
				m->renderChunkWireframe();
			}
		}
	}
	else {
		for (Chunk* m : *m_sMap->getChunkList()) {
			if (m->isVisible()) {
				m->renderChunk();
			}
		}
	}

	Shader::setBool("useLight", false);

	for (Uint16 i = 0; i < m_sMap->getChunkList()->size(); i++) {
		if (getChunk(i)->isVisible())
			m_sMap->getChunk(i)->renderOutline(m_outline ? static_cast<Chunk::OutlineType>(m_nameList->getItem(i).state + 1) : Chunk::OutlineType::NONE);
	}

	if (m_grid) renderGrid();

	switch (MTool::getTool(m_tool)->getType()) {
	case Tool::ToolType::VOXEL:
		static_cast<VoxelTool*>(MTool::getTool(m_tool))->render();
		break;
	case Tool::ToolType::MATRIX:
		static_cast<ChunkTool*>(MTool::getTool(m_tool))->render();
		break;
	default: break;
	}
}
void Map::renderEditorShadow() {
	for (Chunk* m : *m_sMap->getChunkList()) {
		m->renderShadow();
	}
}
void Map::renderGrid() {
	GLfloat gc = GPreferences::getGridCount();	// Grid Count
	GLfloat gs = GPreferences::getGridCount() * GPreferences::getGridSpace();	// Grid Space
	Shader::pushChunkMap();
	Shader::transformMap(glm::translate(glm::mat4(), glm::vec3(0.f, -0.025f, 0.f)));
	Shader::applyMap();
	Color& gridColor = Component::getElementColor("mapGrid");
	glm::vec4 colorVec = glm::vec4(gridColor.r, gridColor.g, gridColor.b, gridColor.a);
	glm::vec4 colorVecInset = colorVec;
	colorVecInset.a *= 0.5f;
	Shader::setColor(glm::vec4(1.f));
	for (Sint32 i = -gs; i <= gs; i += GPreferences::getGridSpace()) {
		if (i == 0) {
			MMesh::renderLine(glm::vec3(-gs, 0, 0), glm::vec3(0, 0, 0), colorVec);
			MMesh::renderLine(glm::vec3(0, 0, -gs), glm::vec3(0, 0, 0), colorVec);

			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(gs, 0, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, gs, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			MMesh::renderLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, gs), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else {
			if ((i / GPreferences::getGridSpace()) % 2 == 0) {
				MMesh::renderLine(glm::vec3(i, 0, -gs), glm::vec3(i, 0, gs), colorVec);
				MMesh::renderLine(glm::vec3(-gs, 0, i), glm::vec3(gs, 0, i), colorVec);
			}
			else {
				MMesh::renderLine(glm::vec3(i, 0, -gs), glm::vec3(i, 0, gs), colorVecInset);
				MMesh::renderLine(glm::vec3(-gs, 0, i), glm::vec3(gs, 0, i), colorVecInset);
			}
		}
	}
	Shader::popChunkMap();
}

std::vector<std::string> Map::getChunkNames() {
	return m_sMap->getChunkNames();
}
Chunk* Map::getChunk(Sint16 id) {
	if (id < 0 || id >= Sint16(m_sMap->getChunkList()->size())) return 0;
	return m_sMap->getChunk(id);
}
Chunk* Map::getChunk(std::string p_name) {
	for (Chunk* m : *m_sMap->getChunkList()) {
		if (m->getName() == p_name) return m;
	}
	return 0;
}
std::vector<Chunk*> Map::getVisibleMatrices() {
	std::vector<Chunk*> matrices;
	for (auto li : m_nameList->getItemList()) {
		if (li.visible) {
			matrices.push_back(getChunk(li.name));
		}
	}
	return matrices;
}
Chunk* Map::getSelectedChunk() {
	if (m_nameList->getSelectedItem() != -1) {
		return getChunk(m_nameList->getSelectedItem());
	}
	return 0;
}
std::vector<Chunk*> Map::getSelectedMatrices() {
	std::vector<Chunk*> matrices;
	for (auto li : m_nameList->getItemList()) {
		if (li.state == 2) {
			matrices.push_back(getChunk(li.name));
		}
	}
	return matrices;
}
glm::vec3 Map::getSelectedMatricesCenter() {
	GLfloat bn = 1e8;
	glm::vec3 minBound = { bn, bn, bn }, maxBound = { -bn, -bn, -bn };
	glm::vec3 center;
	for (Chunk* m : getSelectedMatrices()) {
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

void Map::addImageChunk(std::string p_filename) {
	Texture* tex = MTexture::getTextureExternal(p_filename);
	Sint32 mid = addChunk(p_filename.substr(p_filename.find_last_of('\\') + 1),
		Camera::getFocus() - (glm::vec3(tex->getSize().x, tex->getSize().y, 1) / 2.f),
		glm::ivec3(tex->getSize().x, tex->getSize().y, 1));
	Chunk* matrix = getChunk(mid);
	for (Sint32 x = 0; x < tex->getSize().x; x++) {
		for (Sint32 y = 0; y < tex->getSize().y; y++) {
			Uint8 r = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 0];
			Uint8 g = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 1];
			Uint8 b = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 2];
			Uint8 a = tex->getGlfwImage()->pixels[static_cast<Sint32>(x + y * tex->getSize().x) * 4 + 3];
			if (a) {
				matrix->setVoxel(glm::ivec3(x, tex->getSize().y - y - 1, 0),
					Voxel(1, Color(r, g, b)));
			}
		}
	}
}

void Map::useScriptColorPalette() {
	m_scriptColor[0] = m_voxelColor->r;
	m_scriptColor[1] = m_voxelColor->g;
	m_scriptColor[2] = m_voxelColor->b;
}
Sint32 Map::addScriptChunk(std::string p_name, Sint32 width, Sint32 height, Sint32 depth) {
	Sint32 id = Editor::getMap()->addChunk(p_name, glm::ivec3(), glm::ivec3(width, height, depth));
	setScriptChunkSelected(id);
	return id;
}
Sint32 Map::getScriptChunkByName(std::string p_name) {
	return Editor::getMap()->getChunk(p_name)->getId();
}
void Map::setScriptChunkPos(Sint32 p_matrix, GLfloat x, GLfloat y, GLfloat z) {
	m_scriptChunk->setPosition(glm::vec3(x, y, z));
}
void Map::setScriptChunkSize(Sint32 p_matrix, Sint32 w, Sint32 h, Sint32 d) {
	m_scriptChunk->setSize(glm::ivec3(w, h, d));
}
void Map::setScriptChunkSelected(Sint32 id) {
	Editor::getMap()->setSelectedChunk(id);
	m_scriptChunk = Editor::getMap()->getEditChunk()->getChunk();

	glm::vec3 pos = m_scriptChunk->getPos();
	m_scriptPos[0] = pos.x;
	m_scriptPos[1] = pos.y;
	m_scriptPos[2] = pos.z;

	glm::ivec3 size = m_scriptChunk->getSize();
	m_scriptSize[0] = size.x;
	m_scriptSize[1] = size.y;
	m_scriptSize[2] = size.z;
}
void Map::getScriptColor(Sint32 x, Sint32 y, Sint32 z) {
	Color& voxelColor = m_scriptChunk->getVoxel(glm::ivec3(x, y, z)).color;
	m_scriptColor[0] = voxelColor.r;
	m_scriptColor[1] = voxelColor.g;
	m_scriptColor[2] = voxelColor.b;
}
void Map::setScriptVoxel(Sint32 x, Sint32 y, Sint32 z, Sint32 p_interAction) {
	if (p_interAction) {
		m_scriptChunk->setVoxel(glm::ivec3(x, y, z),
			Voxel(p_interAction, Color(m_scriptColor[0], m_scriptColor[1], m_scriptColor[2])));
	}
	else {
		m_scriptChunk->setVoxel(glm::ivec3(x, y, z),
			Voxel(p_interAction, Color()));
	}
}
void Map::loadScriptFile(std::string p_scriptFile) {
	if (Editor::isMap() && Editor::getMap()->getEditChunk()->getId() != -1) {
		setScriptChunkSelected(Editor::getMap()->getEditChunk()->getId());
		GLua::loadScriptFile(p_scriptFile);
		Editor::getMap()->getEditChunk()->reset(true);
	} else {
		if (!Editor::isMap()) {
			Logger::logError("Map is not currently loaded");
		} else {
			Logger::logError("No selected matrix found!");
		}
	}
}
void Map::loadScriptString(std::string p_script) {
	if (Editor::isMap() && Editor::getMap()->getSelectedChunk()) {
		setScriptChunkSelected(Editor::getMap()->getEditChunk()->getId());
		GLua::loadScriptString(p_script);
		Editor::getMap()->getEditChunk()->reset(true);
	} else {
		if (!Editor::isMap()) {
			Logger::logError("Map is not currently loaded");
		} else {
			Logger::logError("No selected matrix found!");
		}
	}
}

bool Map::exitSave() {
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
	ofn.lpstrFilter = "Map (*.csm)\0*.csm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save As";

	if (!GetSaveFileName(&ofn)) return false;
	LFormat::save(filename, m_sMap->getChunkList());
	return true;
}
void Map::autosave() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Chronovox Studio\\autosave.csm");
	CsmFormat::save(documents, m_sMap->getChunkList());
}
bool Map::autoload() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	strcat_s(documents, "\\Chronovox Studio\\autosave.csm");
	return loadOpen(documents);
}
void Map::add() {
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
	ofn.lpstrFilter = "Map (*.csm, *.nvm, *.qb, *.png)\0"
		"*.csm;*.nvm;*.qb;*.png*\0"
		//"Animation (*.nva)\0"
		//"*.nva\0"
		"Any File\0"
		"*.*\0\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Add Map";
	if (!GetOpenFileName(&ofn)) return;
	loadAdd(filename);
	updateChunkList();
}
void Map::newMap() {
	if (m_sMap->getChunkList()->size() > 0) {
		save();
		for (Chunk* m : *m_sMap->getChunkList()) {
			delete m;
		}
		m_sMap->getChunkList()->clear();
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearChunk(false);
		m_nameList->clear();
	}
	addChunk("Chunk", { -5, 0, -5 }, { 10, 10, 10 });
}
bool Map::save() {
	if (m_directory != "") {
		LFormat::save(getPath(), m_sMap->getChunkList());
		m_matrixEdit->setChanged(false);
		return true;
	}
	else {
		return saveAs();
	}
	return false;
}
bool Map::saveAs() {
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

	LFormat::save(filepath, m_sMap->getChunkList());
	std::string fp = filepath;
	GRecents::renameFile(getPath(), fp);
	setDirectory(fp.substr(0, fp.find_last_of('\\') + 1));
	setName(filename);
	m_matrixEdit->setChanged(false);
	return true;
}
bool Map::loadOpen(std::string p_fileName) {
	LFormat::ImportType _formatType = LFormat::valid(p_fileName);
	if (_formatType != LFormat::ImportType::NONE) {
		m_matrixEdit->clearCommands();
		m_matrixEdit->clearChunk(false);
		m_nameList->clear();
		m_sMap->getChunkList()->clear();
		setPath(p_fileName);
		GRecents::addFile(p_fileName);
		GFormat::setLoadFunction([&]() { 
			updateChunkList();
			m_matrixEdit->setChanged(false);
			setBuilding(false);
			});
		LFormat::load(p_fileName, m_sMap->getChunkList(), _formatType);
		return true;
	}
	return false;
}
bool Map::loadAdd(std::string p_fileName) {
	LFormat::ImportType _formatType = LFormat::valid(p_fileName);
	if (_formatType != LFormat::ImportType::NONE) {
		setBuilding(true);
		GFormat::setLoadFunction([&]() {
			updateChunkList();
			m_matrixEdit->setChanged(true);
			setBuilding(false);
			});
		LFormat::load(p_fileName, m_sMap->getChunkList(), _formatType);
		return true;
	}
	return false;
}
bool Map::exportAs(LFormat::ExportType p_exportType) {
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

	LFormat::exportFile(filepath, m_sMap->getChunkList(), p_exportType);
	return true;
}

void Map::fileNew() {
	m_matrixEdit->clearChunk();
	m_matrixEdit->setCommandChaining(true);
	for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
		m_matrixEdit->addCommand(new MDeleteCommand(m_sMap->getChunk(i), *m_sMap->getChunkList(), i, m_nameList));
		m_nameList->removeItem(i);
		m_sMap->getChunkList()->erase(m_sMap->getChunkList()->begin() + i);
	}
	m_matrixEdit->setCommandChaining(false);
	updateChunkList();
}
void Map::fileAdd() {
	add();
}
bool Map::fileSave() {
	return save();
}
bool Map::fileSaveAs() {
	return saveAs();
}
void Map::fileExit() {

}
void Map::fileExport(LFormat::ExportType p_exportType) {
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

void Map::editMergeChunk() {
	std::vector<Chunk*> matrices = getSelectedMatrices();
	if (matrices.size() > 1) {
		m_matrixEdit->setCommandChaining(true);
		m_matrixEdit->reset(false);
		Chunk* selected = new Chunk(*m_sMap->getChunk(matrices[0]->getId()));
		for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
			if (m_nameList->getItem(i).state == 2 && i != matrices[0]->getId()) {
				m_sMap->getChunk(matrices[0]->getId())->merge(*m_sMap->getChunk(i));
			}
		}
		m_matrixEdit->addCommand(new MResizeCommand(m_sMap->getChunk(matrices[0]->getId()), selected));
		for (Sint16 i = m_nameList->getItemCount() - 1; i >= 0; i--) {
			if (m_nameList->getItem(i).state == 2 && i != matrices[0]->getId()) {
				m_matrixEdit->addCommand(new MDeleteCommand(m_sMap->getChunk(i), *m_sMap->getChunkList(), i, m_nameList));
				m_nameList->removeItem(i);
				m_sMap->getChunkList()->erase(m_sMap->getChunkList()->begin() + i);
			}
		}
		m_matrixEdit->setCommandChaining(false);
		m_matrixEdit->reset(false);
		updateChunkList();
		selectChunk(matrices[0]->getId());
		Logger::logNormal("Merged matrices");
	}
	else {
		Logger::logNormal("Must select more than one matrices to merge");
	}
}
void Map::editNewChunk() {
	/*
	NewMapDialog::getInstance().getDialog()->setOptionFunc("Create", [&]() {
		CDialog* nmd = NewMapDialog::getInstance().getDialog();
		if (nmd->findComponent("WIDTH")->getValue() > 0
			&& nmd->findComponent("HEIGHT")->getValue() > 0
			&& nmd->findComponent("DEPTH")->getValue() > 0) {
			glm::vec3 pos = glm::vec3(nmd->findComponent("WIDTH")->getValue() / -2,
				nmd->findComponent("HEIGHT")->getValue() / -2,
				nmd->findComponent("DEPTH")->getValue() / -2) + Camera::getFocus();
			this->addChunk(nmd->findComponent("MATRIXNAME")->getTitle() == "" ? "Chunk" : nmd->findComponent("MATRIXNAME")->getTitle(), pos,
				glm::ivec3(nmd->findComponent("WIDTH")->getValue(),
					nmd->findComponent("HEIGHT")->getValue(),
					nmd->findComponent("DEPTH")->getValue()));
		}
		nmd->setActive(false);
	});
	*/
	//Gui::openDialog(NewMapDialog::getInstance().getDialog());
	Gui::openDialog("NewChunk.lua");
	Gui::topDialog()->addFunction([&]() {
		std::string name = Gui::topDialog()->findComponent("MATRIXNAME")->getTitle();
		GLfloat width	 = Gui::topDialog()->findComponent("SIZE0")->getValue();
		GLfloat height	 = Gui::topDialog()->findComponent("SIZE1")->getValue();
		GLfloat depth	 = Gui::topDialog()->findComponent("SIZE2")->getValue();
		selectChunk(addChunk(name,
			glm::vec3(width / -2, height / -2, depth / -2) + Camera::getFocus(),
			glm::ivec3(width, height, depth)));
		});
}
void Map::editChunkProperties() {
	Chunk* m = m_matrixEdit->getChunk();
	if (!m) return;

	Gui::openDialog("ChunkProperties.lua");
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
		renameChunk(m_matrixEdit->getChunk()->getId(), Gui::topDialog()->findComponent("NAME")->getTitle());
		m_matrixEdit->getChunk()->setParent(Gui::topDialog()->findComponent("PARENT")->getTitle());
		m_matrixEdit->getChunk()->setPosition(glm::vec3(	Gui::topDialog()->findComponent("POS0")->getValue(),
															Gui::topDialog()->findComponent("POS1")->getValue(),
															Gui::topDialog()->findComponent("POS2")->getValue()));
		m_matrixEdit->getChunk()->setSize(glm::ivec3(	Gui::topDialog()->findComponent("SIZE0")->getValue(),
														Gui::topDialog()->findComponent("SIZE1")->getValue(),
														Gui::topDialog()->findComponent("SIZE2")->getValue()));
		getEditChunk()->addCommand(new MResizeCommand(getEditChunk()->getChunk(), getEditChunk()->getInitChunk()));
		getEditChunk()->reset(false);
		});
}
void Map::editCopy() {
	copyChunk();
}
void Map::editCut() {
	copyChunk();
	deleteSelectedMatrices();
}
void Map::editPaste() {
	pasteChunk();
}
void Map::editSelectAll() {
	selectAll();
}
void Map::editUndo() {
	undo();
	//updateChunkList();
}
void Map::editRedo() {
	redo();
	//updateChunkList();
}

void Map::matrixSize() {
	Chunk* m = m_matrixEdit->getChunk();
	if (!m) return;

	Gui::openDialog("ResizeChunk.lua");
	Gui::topDialog()->findComponent("SIZE0")->setValue(m->getSize().x);
	Gui::topDialog()->findComponent("SIZE1")->setValue(m->getSize().y);
	Gui::topDialog()->findComponent("SIZE2")->setValue(m->getSize().z);
	Gui::topDialog()->addFunction([&]() { 
		m_matrixEdit->reset(true);
		m_matrixEdit->getChunk()->scale(glm::vec3(	Gui::topDialog()->findComponent("SIZE0")->getValue() / m_matrixEdit->getChunk()->getSize().x,
													Gui::topDialog()->findComponent("SIZE1")->getValue() / m_matrixEdit->getChunk()->getSize().y,
													Gui::topDialog()->findComponent("SIZE2")->getValue() / m_matrixEdit->getChunk()->getSize().z),
			glm::vec3());
		m_matrixEdit->addCommand(new MResizeCommand(m_matrixEdit->getChunk(), m_matrixEdit->getInitChunk()));
		m_matrixEdit->reset(false);
		});
}

void Map::generateCustom() {
	Gui::openDialog("Generate.lua");
	Gui::topDialog()->addFunction([&]() {
		m_matrixEdit->reset(true);
		loadScriptString(Gui::topDialog()->findComponent("SCRIPT")->getTitle());
		m_matrixEdit->reset(false);
		});
}
