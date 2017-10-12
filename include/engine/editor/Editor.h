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
#include <thread>

class Editor
{
public:
	enum EngineState {
		STARTING = 0,
		RUNNING = 1,
		CLOSING = 2
	} m_engineState;

	Editor();
	bool attemptClose();
	~Editor();

	void resize();

	void dropFile(const char* path);

	void renderMouse();

	void input();
	void update();
	void render3d();
	void render2d();
	static Model* getModel() { return m_model; }
private:
	static Model* m_model;
	GLfloat m_lastUpdate, m_deltaUpdate;
	Vector2<Sint32> m_mouseBuffer;
	Sint32 r, g, b;
	Container* m_mainGui;
	std::thread *m_autosaveThread;
};
