#pragma once

#include "..\..\command\Command.h"
#include "..\Matrix.h"

struct MRenameCommand : public Command {
public:
	MRenameCommand(Matrix* m, std::string p_from, std::string p_to);
	void terminate();

	void undo();
	void redo();

private:
	Sint32 m_matrix;

	std::string from, to;
};
