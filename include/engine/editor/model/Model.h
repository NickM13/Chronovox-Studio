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
	Vector3<GLfloat> m_point, m_scalePos; // Point of selection, move/scale drag offsets
	Vector3<Sint32> m_selectedVoxel; // Selected voxel coord
	Vector3<Sint32> m_selectedVoxelOffset; // Voxel off of selected face
	Voxel m_boxVoxel; // Voxel filling box area
	Sint8 m_selectedSide; // Face selected

	Vector3<GLfloat> m_pos;
	Vector3<GLfloat> m_size;
	Sint16 m_hoverMatrix;
	Vector3<GLfloat> m_grabStart, m_grabCurrent;
	GLfloat m_dragDifference;
	Sint8 m_selectedScale;

	EditMatrix *m_matrixEdit;

	Vector3<GLfloat> dir, pos, norm;
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

	void setVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos, Voxel p_voxel);
	Voxel getVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos);
	Uint16 getVoxelId(Uint16 p_matrix, Vector3<Sint32> p_pos);

	void resize(Uint16 p_matrixId, Vector3<Sint32> p_offset, Vector3<Sint32> p_size);
	void shiftMatrix(Vector3<Sint32> p_direction);
	void flipMatrix(Sint8 p_axesFlags);
	void rotateMatrix(Sint8 p_axesFlags);
	void addMatrix(std::string p_name, Vector3<GLfloat> p_pos, Vector3<Sint16> p_size);
	void renameMatrix(Uint16 id, std::string p_name);
	void deleteSelectedMatrices();
	void moveMatrix(bool up);
	void hoverMatrix(Sint16 id);
	void selectMatrix(Sint16 id);

	Vector3<GLfloat> getPos() { return m_pos; }
	Vector3<GLfloat> getSize() { return m_size; }

	void updateMatrixList();

	void inputEditor(Sint8 p_guiFlags);
	void updateEditor(GLfloat p_deltaUpdate);
	void renderEditor();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);
	Matrix* getMatrix(std::string p_name);
	std::vector<Matrix*> getMatrixList() { return m_matrices; }
	Matrix* getSelectedMatrix();
	std::vector<Matrix*> getSelectedMatrices();
	Vector3<GLfloat> getSelectedMatricesCenter();
	
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
