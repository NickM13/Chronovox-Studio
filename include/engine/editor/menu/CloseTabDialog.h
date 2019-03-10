#pragma once

#include "engine\gfx\gui\LGui.h"

class CloseTabDialog : public Singleton<CloseTabDialog> {
private:
	CDialog* m_dialog;

public:
	CloseTabDialog();
	CDialog* getDialog();
};
