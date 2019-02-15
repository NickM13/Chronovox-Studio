#pragma once

#include "engine\gfx\LGui.h"

class CloseTabDialog : public Singleton<CloseTabDialog> {
private:
	CDialog* m_dialog;

public:
	CloseTabDialog();
	CDialog* getDialog();
};
