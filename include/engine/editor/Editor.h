#pragma once
#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\utils\global\GScreen.h"
#include "engine\utils\LOpenGL.h"
#include "engine\utils\variable\datatype\Rectangle.h"

#include "engine\gfx\gui\LGui.h"

#include "model\Model.h"

#include <vector>
#include <thread>
#include <mutex>

class Editor {
public:
	enum EditorState {
		STARTING,
		RUNNING,
		STOPPING
	};
	enum EditorMode {
		NONE,
		MODEL,
		ANIMATION
	};
private:
	GLuint gBuffer, gPosition, gNormal, gAlbedoSpec, rboDepth;
	GLuint aaBuffer, aaScreenTex, rboAA;
	GLuint ssaoBuffer, ssaoColorBuffer;
	GLuint quadVAO = 0, cubeVAO = 0;
	GLuint quadVBO, cubeVBO = 0;
	Matrix* m_matrixCopy = 0; // Copy/paste matrix

	struct Project {
		EditorMode mode = {};
		TEMode* editor = 0;
		Project(EditorMode p_mode, TEMode* p_editor) : mode(p_mode), editor(p_editor) {}
	};
	static std::vector<Project*> m_projects;
	static CTabBar* m_projectTabs;
	static Project* m_cProj;

	static std::string* m_dataString;

	static EditorState m_editorState;
	static EditorMode m_editorMode;

	glm::vec3 m_sunlightDirection;

	GLfloat m_lastUpdate = 0.f, m_deltaUpdate = 0.f;
	std::thread *m_autosaveThread;
	Sint32 m_autosavePeriod;
	std::mutex m_autosaveMutex;
	std::condition_variable m_autosaveCv;

	void closeProject(Sint32 p_index);

	bool initGBuffer();
	bool initAABuffer();
	void terminateGBuffer();
	void terminateAABuffer();

	void renderSquare();
	void renderCube();
public:
	Editor();
	~Editor();
	bool attemptClose();

	static bool isAnimation()	{ return (m_cProj && m_cProj->mode == EditorMode::ANIMATION); }
	static bool isModel()		{ return (m_cProj && m_cProj->mode == EditorMode::MODEL); }

	static Model* getModel() {
		if (m_cProj && m_cProj->mode == EditorMode::MODEL) {
			return (Model*)m_cProj->editor;
		}
		return 0;
	}

	static void setDataString(std::string* p_dataString);

	static void setEditorState(EditorState p_state);
	static EditorState getEditorState();

	static void setEditorMode(EditorMode p_mode);
	static EditorMode getEditorMode();

	void newProject(Project* p_project);
	void setProject(Sint32 p_index);
	void closeSelectedProject();
	void attemptCloseProject(Sint32 p_index);

	void resize();

	void dropFile(const char* path);

	void renderMouse();

	void bindGBuffer();

	glm::vec3 getSunlightDir();
	glm::mat4 getSunlightMatrix();

	void input();
	void update();
	void render2d();
	void renderGeometry();
	void renderLight();

	void copyMatrix();
	void cutMatrix();
	void pasteMatrix();

	/* Menubar functions */

	bool fileNewModel();
	bool fileOpen();
	bool fileSave();
	bool fileSaveAs();
	bool fileExit();
	
	void editUndo();
	void editRedo();
	void editPreferences();

	void helpAbout();
};
