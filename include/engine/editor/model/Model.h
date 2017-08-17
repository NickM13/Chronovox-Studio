#pragma once

#define GRAVITY 4

#include "EditMatrix.h"
#include "Matrix.h"

#include "tool\Box.h"
#include "tool\Fill.h"
#include "tool\MatrixCast.h"
#include "tool\MatrixMove.h"
#include "tool\MatrixResize.h"

#include "..\command\DeleteCommand.h"
#include "..\command\DrawCommand.h"
#include "..\command\MoveCommand.h"
#include "..\command\NewCommand.h"
#include "..\command\ResizeCommand.h"

#include "engine\utils\variable\datatype\Color.h"
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector2.h"
#include "engine\utils\variable\datatype\Vector3.h"

#include "engine\gfx\LTexture.h"
#include "engine\gfx\mesh\object\TMesh.h"

#include "format\NvmFormat.h"

#include <vector>
#include <iostream>

enum Tool
{
	ADD = 0,
	ERASE,
	REPLACE,
	EYEDROP,
	SELECT,
	MOVE,
	RESIZE
};

class Model
{
public:
	Model();
	~Model();

	void setDataString(std::string* p_dataString) { m_dataString = p_dataString; }
	void setToolVar(Uint16* p_tool, Uint16* p_toolMeta) { m_tool = p_tool; m_toolMeta = p_toolMeta; }
	void setTool(Tool p_tool) { *m_tool = p_tool; *m_toolMeta = 0; }
	void setToolMeta(Uint16 meta = 0) { if(meta < 3 && *m_tool < EYEDROP) *m_toolMeta = meta; }
	void setColor(Sint32& p_r, Sint32& p_g, Sint32& p_b);
	Sint16* getSelectedMatrix() { return &m_matrixEdit->getId(); }
	std::vector<std::string> *getMatrixList() { return &m_matrixList; }

	void toggleGrid();
	void toggleOutline();

	void zoom(GLfloat p_scroll);
	void turn(Vector2<Sint32> p_mouseMove);
	void pan(Vector2<Sint32> p_mouseMove);

	void focus();

	void undo();
	void redo();
	void removeMatrix();

	void copyMatrix();
	void pasteMatrix();

	void setVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos, Voxel p_voxel);
	Voxel getVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos);
	Uint16 getVoxelId(Uint16 p_matrix, Vector3<Sint32> p_pos);

	void resize(Uint16 p_matrixId, Vector3<Sint32> p_offset, Vector3<Sint32> p_size);
	void shiftMatrix(Vector3<Sint32> p_direction);
	void flipMatrix(Sint8 p_axesFlags);
	void addMatrix(std::string p_name, Vector3<GLfloat> p_pos, Vector3<Sint16> p_size);
	void renameMatrix(Uint16 id, std::string p_name);

	Vector3<GLfloat> getCamPosition();
	Vector3<GLfloat> getCamDirection();
	// Note: For some reason this only works with fov=70
	Vector3<GLfloat> getCamMouseDirection();

	void input(Sint8 p_guiFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
	void renderSelected();
	void renderGrid();
	void renderFocus();
	void renderSkyBox();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);

	void save();
	void open();
	void newModel();

	void save(std::string p_fileName);
	void load(std::string p_fileName);
private:
	// Editting tool, tool mode
	Uint16* m_tool, *m_toolMeta;

	// Shared variables color
	Sint32 *r, *g, *b;
	std::vector<std::string> m_matrixList;

	// Visual guidelines
	std::string* m_dataString; // Info bar at bottom of screen
	bool m_grid;
	bool m_outline;

	Texture m_skyTexture;

	// Matrices
	std::vector<Matrix*> m_matrices;
	Matrix* m_matrixCopy; // Copy/paste matrix

	// Camera variables
	Vector3<GLfloat> m_camPos;
	Vector3<GLfloat> m_camRot;
	GLfloat m_zoom, m_tarZoom, m_zoomSpeed;

	// Editting tool variables
	Vector3<GLfloat> m_point, m_scalePos; // Point of selection, move/scale drag offsets
	Vector3<Sint32> m_selectedVoxel; // Selected voxel coord
	Vector3<Sint32> m_selectedVoxelOffset; // Voxel off of selected face
	Voxel m_boxVoxel; // Voxel filling box area
	Sint8 m_selectedSide; // Face selected
	
	Sint16 m_hoverMatrix;
	Sint16 m_selectedMatrix;
	Vector3<GLfloat> m_grabStart, m_grabCurrent;
	GLfloat m_dragDifference;
	Sint8 m_selectedScale;
	Matrix* m_dragMatrix;
	enum DragType
	{
		NONE = 0,
		MAKEBOX,
		MOVING,
		RESIZING
	} m_dragging;

	EditMatrix *m_matrixEdit;

	Vector3<GLfloat> dir, pos, norm;
	GLfloat dist, denom, p;
};
