#pragma once

#include "engine\gfx\gui\LGui.h"

class CloseEditorDialog : public Singleton<CloseEditorDialog> {
private:
	CDialog* m_dialog;

public:
	CloseEditorDialog();
	CDialog* getDialog();
};
