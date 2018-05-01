#pragma once
#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Rectangle.h"

#include "engine\gfx\LGui.h"

#include "animation\Animation.h"
#include "model\Model.h"

#include <vector>
#include <thread>

class Editor {
public:
	enum EditorState {
		STARTING = 0,
		RUNNING = 1,
		CLOSING = 2
	};
	enum EditorMode {
		MODEL = 0,
		ANIMATION = 1
	};
private:
	struct ShadowBuffer {
		GLuint framebufferName, renderedTexture, depthRenderbuffer;
		glm::vec2 shadowSize;
	} m_shadowBuffer;

	static Animation* m_animation;
	static Model* m_model;
	static TEMode* m_tMode; // Template pointer to current EngineMode variable, to compress EditorOverlay function lines

	static EditorState m_editorState;
	static EditorMode m_editorMode;

	static Container* m_mainGui;

	glm::vec3 m_sunlightDirection;

	GLfloat m_lastUpdate, m_deltaUpdate;
	std::thread *m_autosaveThread;

	bool initShadowBuffer();
	void terminateShadowBuffer();
public:
	Editor();
	~Editor();
	bool attemptClose();

	static Animation* getAnimation() { return m_animation; }
	static Model* getModel() { return m_model; }

	static void setEditorState(EditorState p_state);
	static EditorState getEditorState();

	static void setEditorMode(EditorMode p_mode);
	static EditorMode getEditorMode();

	void resize();

	void dropFile(const char* path);

	void renderMouse();

	void bindShadowBuffer();
	void unbindShadowBuffer();
	void bindShadowTexture();
	void renderShadowTexture();

	glm::vec3 getSunlightDir();
	glm::mat4 getSunlightMatrix();

	void input();
	void update();
	void renderShadow();
	void render3d();
	void render2d();

	// Overlay functions
	void fileNew();
	void fileOpen();
	void fileSave();
	void fileExit();
	
	void editUndo();
	void editRedo();
};
