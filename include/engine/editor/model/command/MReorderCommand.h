#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"
#include "engine\gfx\gui\LGui.h"

struct MReorderCommand : public Command {
private:
	std::vector<Matrix*>* m_matrices;
	std::vector<CList::ListItem> m_from, m_to;

	CList* m_nameList;

	void reorder();

public:
	MReorderCommand(std::vector<Matrix*>* p_matrices, std::vector<CList::ListItem> p_order, CList* p_nameList);
	void terminate();

	void undo();
	void redo();
};
