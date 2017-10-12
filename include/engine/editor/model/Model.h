#pragma once

#define GRAVITY 4

#include "EditMatrix.h"
#include "Matrix.h"

#include "tool\Box.h"
#include "tool\Fill.h"
#include "tool\MatrixCast.h"
#include "tool\MatrixMove.h"
#include "tool\MatrixResize.h"
#include "tool\Select.h"

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

#include "format\Format.h"

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
	void init();
	~Model();

	void setDataString(std::string* p_dataString) { m_dataString = p_dataString; }
	void setToolVar(Uint16* p_tool, Uint16* p_toolMeta) { m_tool = p_tool; m_toolMeta = p_toolMeta; }
	void setTool(Tool p_tool) { *m_tool = p_tool; *m_toolMeta = 0; }
	void setToolMeta(Uint16 meta = 0) { if(meta < 3 && *m_tool < EYEDROP) *m_toolMeta = meta; }
	void setColor(Sint32& p_r, Sint32& p_g, Sint32& p_b);
	void updateLists();

	void toggleGrid();
	void toggleOutline();

	void zoom(GLfloat p_scroll);
	void turn(Vector2<Sint32> p_mouseMove);
	void pan(Vector2<Sint32> p_mouseMove);

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
	void addMatrix(std::string p_name, Vector3<GLfloat> p_pos, Vector3<Sint16> p_size);
	void renameMatrix(Uint16 id, std::string p_name);
	void removeMatrix();
	void deleteSelectedMatrices();
	void selectMatrix(Sint16 id);

	Vector3<GLfloat> getPos() { return m_pos; }
	Vector3<GLfloat> getSize() { return m_size; }
	Vector3<GLfloat> getCamPosition();
	Vector3<GLfloat> getCamDirection();
	Vector3<GLfloat> getCamMouseDirection(); // Note: For some reason this only works with fov=70
	GLfloat castRay(Vector3<GLfloat> p_start, Vector3<GLfloat> p_direction); // Cast ray, return exact distance from closest voxel - used in Shader

	void input(Sint8 p_guiFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
	void renderSkyBox();

	std::vector<std::string> getMatrixNames();
	Matrix* getMatrix(Sint16 id);
	
	bool exitSave();
	void autosave();
	bool autoload();
	void save();
	void open();
	void newModel();

	void save(std::string p_fileName);
	bool load(std::string p_fileName);
private:
	void renderSelected();
	void renderGrid();
	void renderFocus();

	std::string m_modelName;

	// Editting tool, tool mode
	Uint16* m_tool, *m_toolMeta;

	// Shared variables color
	Sint32 *r, *g, *b;

	// Visual guidelines
	std::string* m_dataString; // Info bar at bottom of screen
	bool m_grid;
	bool m_outline;

	Texture m_skyTexture;

	// Matrices
	std::vector<Matrix*> m_matrices;
	Matrix* m_matrixCopy; // Copy/paste matrix
	CList* m_nameList;

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

	Vector3<GLfloat> m_pos;
	Vector3<GLfloat> m_size;
	Sint16 m_hoverMatrix;
	Vector3<GLfloat> m_grabStart, m_grabCurrent;
	GLfloat m_dragDifference;
	Sint8 m_selectedScale;
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
