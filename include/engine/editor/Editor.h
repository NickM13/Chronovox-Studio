#pragma once

#include "engine\gfx\font\Font.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\OpenGL.h"
#include "engine\utils\variable\datatype\Rectangle.h"
#include "engine\utils\variable\manager\TextureManager.h"
#include "engine\gfx\LTexture.h"

#include "engine\gfx\LGui.h"

#include "model\Model.h"

#include <vector>

class Editor
{
public:
	enum EngineState
	{
		MENU = 0,
		GAME = 1
	} m_engineState;

	enum PauseState
	{
		NONE = 0
	} pauseState;

	Editor();
	~Editor();

	void resize(bool p_maximizedByDrag);

	void setEngineState(EngineState p_state);

	void dropFile(const char* path);

	void renderMouse();

	void input();
	void update();
	void render3d();
	void render2d();
private:
	static Model* m_model;

	GLfloat m_lastUpdate, m_deltaUpdate;
	Vector2<Sint32> m_mouseBuffer;

	Sint32 r, g, b;

	Container m_mainGui;
};
