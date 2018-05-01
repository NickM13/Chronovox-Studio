#pragma once
#include "engine\utils\Utilities.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\mesh\MMesh.h"
#include "camera\Camera.h"

class TEMode {
protected:
	virtual void inputEditor(Sint8 p_guiFlags);
	virtual void updateEditor(GLfloat p_deltaUpdate);
	virtual void renderEditor();
	virtual void renderEditorShadow();
public:
	TEMode();

	void input(Sint8 p_guiFlags);
	void update(GLfloat p_deltaUpdate);
	void render();
	void renderShadow();

	virtual void fileNew();
	virtual void fileOpen();
	virtual void fileSave();
	virtual void fileExit();

	virtual void editUndo();
	virtual void editRedo();
};
