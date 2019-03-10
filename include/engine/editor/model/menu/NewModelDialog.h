#pragma once

#include "engine\gfx\gui\LGui.h"

class NewModelDialog : public Singleton<NewModelDialog> {
private:
	CDialog* m_dialog;

public:
	NewModelDialog();
	CDialog* getDialog();
};
