#pragma once

#include "engine\utils\Utilities.h"

class Command {
protected:
	std::string m_commandType;
	bool m_staticChange = true;
public:
	virtual void undo() = 0;
	virtual void redo() = 0;
	std::string getCommandType() { return m_commandType; }
	bool isStaticChange() { return m_staticChange; }
};
