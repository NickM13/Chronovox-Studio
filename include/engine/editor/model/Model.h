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
	static Sint32 m_tool;

	// Shared variables color
	static Color* m_voxelColor;
	ColorOverlay* m_colorOverlay = 0;

	// Visual guidelines
	std::string* m_dataString = 0;	// Info bar at bottom of screen
	bool m_grid = false;
	bool m_outline = false;
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

	// Script variables
	static GLfloat m_scriptPos[3];
	static Sint32 m_scriptSize[3];
	static Sint32 m_scriptColor[3];
	static Matrix* m_scriptMatrix;

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
	void updateTool();

	void toggleGrid() { m_grid = !m_grid; }
	bool isGridVisible() const { return m_grid; }
	void toggleOutline() { m_outline = !m_outline; }
	bool isOutlineVisible() const { return m_outline; }
	void toggleWireframe() { m_wireframe = !m_wireframe; }
	bool isWireframe() const { return m_wireframe; }

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
	void scaleMatrix(glm::vec3 p_scale);
	Sint32 addMatrix(std::string p_name, glm::vec3 p_pos, glm::ivec3 p_size);
	void renameMatrix(Uint16 id, std::string p_name);
	void deleteSelectedMatrices();
	void moveMatrix(bool up);
	void hoverMatrix(Sint16 id);
	void setSelectedMatrix(Sint16 id);
	void selectMatrix(Sint16 id);
	EditMatrix* getEditMatrix() const { return m_matrixEdit; }

	glm::vec3 getPos() { return m_pos; }
	glm::vec3 getSize() { return m_size; }

	void updateMatrixList();
	// Sync visibility of matrices using the CList's values
	void updateVisibilityLtoM();
	// Sync visibility of matrices using the Model's values
	void updateVisibilityMtoL();

	void inputEditor(Sint8 p_guiFlags);
	void updateEditor(GLfloat p_deltaUpdate);
	void renderEditor();
	void renderEditorShadow();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);
	Matrix* getMatrix(std::string p_name);
	std::vector<Matrix*> getMatrixList() { return *m_sModel->getMatrixList(); }
	std::vector<Matrix*> getVisibleMatrices();
	Matrix* getSelectedMatrix();
	std::vector<Matrix*> getSelectedMatrices();
	glm::vec3 getSelectedMatricesCenter();

	void addImageMatrix(std::string p_filename);

	static void useScriptColorPalette();
	static Sint32 addScriptMatrix(std::string p_name, Sint32 width, Sint32 height, Sint32 depth);
	static Sint32 getScriptMatrixByName(std::string p_name);
	static void setScriptMatrixPos(Sint32 p_matrix, GLfloat x, GLfloat y, GLfloat z);
	static void setScriptMatrixSize(Sint32 p_matrix, Sint32 x, Sint32 y, Sint32 z);
	static void setScriptMatrixSelected(Sint32 i);
	static void getScriptColor(Sint32 x, Sint32 y, Sint32 z);
	static void setScriptVoxel(Sint32 x, Sint32 y, Sint32 z, Sint32 p_interAction = 1);
	static void loadScriptFile(std::string p_scriptFile);
	static void loadScriptString(std::string p_script);
	
	bool exitSave();
	void autosave();
	bool autoload();
	void add();
	void newModel();

	bool save();
	bool saveAs();
	bool loadOpen(std::string p_fileName);
	bool loadAdd(std::string p_fileName);
	bool exportAs(LFormat::ExportType p_exportType);

	void fileNew();
	void fileAdd();
	bool fileSave();
	bool fileSaveAs();
	void fileExit();
	void fileExport(LFormat::ExportType p_exportType);

	void editMergeMatrix();
	void editNewMatrix();
	void editMatrixProperties();
	void editCopy();
	void editCut();
	void editPaste();
	void editUndo();
	void editRedo();

	void matrixSize();

	void generateCustom();
};
