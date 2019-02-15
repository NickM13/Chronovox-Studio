#pragma once

#include "..\command\LCommand.h"

#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\variable\datatype\Vector3.h"

struct EditMatrix {
private:
	Sint16 m_matrixId;
	Matrix* m_matrix, *m_initMatrix;

	std::vector<std::vector<Command*>> m_recentCommands;
	Uint16 m_commandIndex;
	bool m_commandListIsOpen;
	bool m_commandChaining;
	bool m_changed;

public:
	EditMatrix();
	~EditMatrix();

	bool hasChanged() const { return m_changed; }
	void setChanged(bool p_changed) { m_changed = p_changed; }

	void setCommandListOpen(bool p_isOpen);
	// Set whether new commands are linked (when undoing or redoing, all commands in chain are done)
	void setCommandChaining(bool p_chaining);
	bool isCommandChaining() { return m_commandChaining; }

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
