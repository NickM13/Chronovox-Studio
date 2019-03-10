#pragma once

#include "EditMatrix.h"
#include "..\TEMode.h"
#include "SimpleModel.h"

#include "engine\gfx\gui\LGui.h"

#include "engine\gfx\texture\MTexture.h"
#include "engine\editor\model\menu\ColorOverlay.h"

#include <vector>
#include <iostream>

class Model : public TEMode {
private:
	void renderGrid();

	// Editting tool, tool mode
	static Sint32 m_tool, m_subTool;

	// Shared variables color
	static Color* m_voxelColor;
	ColorOverlay* m_colorOverlay = 0;

	// Visual guidelines
	std::string* m_dataString = 0;	// Info bar at bottom of screen
	bool m_grid = false;
	bool m_outline = false;
	bool m_hideOnSelect = false;	// Visible on select
	bool m_wireframe = false;

	// Matrices
	SimpleModel* m_sModel = 0;
	Matrix* m_matrixCopy = 0; // Copy/paste matrix
	CList* m_nameList = 0;

	// Editting tool variables
	glm::vec3 m_point = {};
	glm::vec3 m_scalePos = {}; // Point of selection, move/scale drag offsets
	static glm::ivec3 m_selectedVoxel; // Selected voxel coord
	static glm::ivec3 m_selectedVoxelOffset; // Voxel off of selected face
	Voxel m_boxVoxel = {}; // Voxel filling box area
	static Sint8 m_selectedSide; // Face selected

	glm::vec3 m_pos = {};
	glm::vec3 m_size = {};
	Sint16 m_hoverMatrix = 0;
	glm::vec3 m_grabStart = {}, m_grabCurrent = {};
	GLfloat m_dragDifference = 0;
	Sint8 m_selectedScale = 0;
	bool m_voxelPlaneMode = false;
	glm::ivec3 m_voxelPlanePos = {};
	Sint8 m_voxelPlaneSide = 0;

	EditMatrix *m_matrixEdit = 0;

	glm::vec3 dir = {}, pos = {}, norm = {};
	GLfloat dist = 0, denom = 0, p = 0;

	void fixSelectedMatrix();
public:
	Model();
	~Model();
	void init();
	void activate();

	bool hasChanged();
	void setChanged(bool p_changed);

	void setDataString(std::string* p_dataString);
	void setTool(Sint32 p_tool);
	void setTool(std::string p_toolName);
	void setSubTool(Sint32 p_subTool = 0);
	void updateTool();

	void toggleGrid();
	bool isGridVisible();
	void toggleOutline();
	bool isOutlineVisible();
	void toggleHideOnSelect();
	bool isHideOnSelect();
	void toggleWireframe();

	void focus();

	void undo();
	void redo();

	void copyMatrix();
	void pasteMatrix();

	void setVoxel(Uint16 p_matrix, glm::ivec3 p_pos, Voxel p_voxel);
	Voxel getVoxel(Uint16 p_matrix, glm::ivec3 p_pos);
	Uint16 getVoxelId(Uint16 p_matrix, glm::ivec3 p_pos);

	void resize(Uint16 p_matrixId, glm::ivec3 p_offset, glm::ivec3 p_size);
	void shiftMatrix(glm::ivec3 p_direction);
	void flipMatrix(Sint8 p_axesFlags);
	void rotateMatrix(Sint8 p_axesFlags);
	void addMatrix(std::string p_name, glm::vec3 p_pos, glm::ivec3 p_size);
	void renameMatrix(Uint16 id, std::string p_name);
	void deleteSelectedMatrices();
	void moveMatrix(bool up);
	void hoverMatrix(Sint16 id);
	void setSelectedMatrix(Sint16 id);
	void selectMatrix(Sint16 id);

	glm::vec3 getPos() { return m_pos; }
	glm::vec3 getSize() { return m_size; }

	void updateMatrixList();

	void inputEditor(Sint8 p_guiFlags);
	void updateEditor(GLfloat p_deltaUpdate);
	void renderEditor();
	void renderEditorShadow();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);
	Matrix* getMatrix(std::string p_name);
	std::vector<Matrix*> getMatrixList() { return *m_sModel->getMatrixList(); }
	Matrix* getSelectedMatrix();
	std::vector<Matrix*> getSelectedMatrices();
	glm::vec3 getSelectedMatricesCenter();
	
	bool exitSave();
	void autosave();
	bool autoload();
	void add();
	void newModel();

	bool save();
	bool saveAs();
	bool loadOpen(std::string p_fileName);
	bool loadAdd(std::string p_fileName);

	void fileNew();
	void fileAdd();
	bool fileSave();
	bool fileSaveAs();
	void fileExit();

	void editNewMatrix();
	void editMatrixProperties();
	void editUndo();
	void editRedo();
};
