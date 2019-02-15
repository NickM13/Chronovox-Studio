#pragma once

#include "engine\gfx\LGui.h"
#include "engine\utils\directory\LDirectory.h"

class AboutDialog : public Singleton<AboutDialog> {
private:
	CDialog* m_dialog;

public:
	AboutDialog();
	CDialog* getDialog();
};
