#pragma once

#include "..\command\LCommand.h"

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector3.h"

struct EditMatrix
{
private:
	Sint16 m_matrixId;
	Matrix* m_matrix, *m_initMatrix;

	std::vector<Command*> m_recentCommands;
	Uint16 m_commandIndex;
	bool m_commandListIsOpen;

public:
	EditMatrix();
	~EditMatrix();

	void setCommandListOpen(bool p_isOpen);

	void addCommand(Command* p_com);
	void undo();
	void redo();
	void clearCommands();

	void reset(bool p_saveChanges = true);

	void setMatrix(Matrix* m, Sint32 id);
	void clearMatrix(bool p_saveChanges = true);
	void saveChanges();
	void refreshPos();

	Voxel getVoxel(glm::ivec3 p_pos);
	Sint16 &getId();
	bool isSet();
	glm::vec3 getPos();
	glm::ivec3 getSize();
	Matrix* getMatrix();
	Matrix* getInitMatrix();
};
