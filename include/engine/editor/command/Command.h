#pragma once

#include "engine\utils\Utilities.h"

struct Command
{
protected:
	bool m_staticChange = true;
public:
	virtual void undo() = 0;
	virtual void redo() = 0;
	bool isStaticChange() { return m_staticChange; }
};
