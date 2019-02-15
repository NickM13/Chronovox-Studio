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
	struct ShadowBuffer {
		GLuint framebufferName, renderedTexture, depthRenderbuffer;
		glm::vec2 shadowSize;
	} m_shadowBuffer;

	struct Project {
		EditorMode mode;
		TEMode* editor;
		Project(EditorMode p_mode, TEMode* p_editor) : mode(p_mode), editor(p_editor) {}
	};
	static std::vector<Project*> m_projects;
	static CTabBar* m_projectTabs;
	static Project* m_cProj;

	static std::string* m_dataString;

	static EditorState m_editorState;
	static EditorMode m_editorMode;

	static Container* m_mainGui;

	glm::vec3 m_sunlightDirection;

	GLfloat m_lastUpdate, m_deltaUpdate;
	std::thread *m_autosaveThread;
	Sint32 m_autosavePeriod;
	std::mutex m_autosaveMutex;
	std::condition_variable m_autosaveCv;

	void closeProject(Sint32 p_index);

	bool initShadowBuffer();
	void terminateShadowBuffer();
public:
	Editor();
	~Editor();
	bool attemptClose();

	static Animation* getAnimation() { if (m_cProj) return (Animation*)m_cProj->editor; return 0; }
	static Model* getModel() { if (m_cProj) return (Model*)m_cProj->editor; return 0; }

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
	bool fileNewModel();
	bool fileNewAnimation();
	bool fileOpen();
	bool fileSave();
	bool fileSaveAs();
	bool fileExit();
	
	void editUndo();
	void editRedo();
};
