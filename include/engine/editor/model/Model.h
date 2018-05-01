#pragma once

#include "EditMatrix.h"
#include "Matrix.h"
#include "..\TEMode.h"

#include "engine\gfx\LGui.h"

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"
#include "engine\editor\model\tool\MTool.h"

#include "engine\gfx\texture\MTexture.h"
#include "engine\editor\model\menu\ColorOverlay.h"

#include "format\Format.h"

#include <vector>
#include <iostream>

class Model : public TEMode {
private:
	void renderGrid();

	std::string m_modelName;

	// Editting tool, tool mode
	Sint32 m_tool, m_subTool;

	// Shared variables color
	Color* m_voxelColor;
	ColorOverlay* m_colorOverlay;

	// Visual guidelines
	std::string* m_dataString; // Info bar at bottom of screen
	bool m_grid;
	bool m_outline;

	// Matrices
	std::vector<Matrix*> m_matrices;
	Matrix* m_matrixCopy; // Copy/paste matrix
	CList* m_nameList;

	// Editting tool variables
	glm::vec3 m_point, m_scalePos; // Point of selection, move/scale drag offsets
	glm::ivec3 m_selectedVoxel; // Selected voxel coord
	glm::ivec3 m_selectedVoxelOffset; // Voxel off of selected face
	Voxel m_boxVoxel; // Voxel filling box area
	Sint8 m_selectedSide; // Face selected

	glm::vec3 m_pos;
	glm::vec3 m_size;
	Sint16 m_hoverMatrix;
	glm::vec3 m_grabStart, m_grabCurrent;
	GLfloat m_dragDifference;
	Sint8 m_selectedScale;

	EditMatrix *m_matrixEdit;

	glm::vec3 dir, pos, norm;
	GLfloat dist, denom, p;
public:
	Model();
	~Model();
	void init();

	void setDataString(std::string* p_dataString);
	void setTool(Sint32 p_tool);
	void setTool(std::string p_toolName);
	void setSubTool(Sint32 p_subTool = 0);
	void updateTool();

	void toggleGrid();
	void toggleOutline();

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
	std::vector<Matrix*> getMatrixList() { return m_matrices; }
	Matrix* getSelectedMatrix();
	std::vector<Matrix*> getSelectedMatrices();
	glm::vec3 getSelectedMatricesCenter();
	
	bool exitSave();
	void autosave();
	bool autoload();
	void save();
	void open();
	void newModel();

	void save(std::string p_fileName);
	bool load(std::string p_fileName);

	void fileNew();
	void fileOpen();
	void fileSave();
	void fileExit();

	void editUndo();
	void editRedo();
};
