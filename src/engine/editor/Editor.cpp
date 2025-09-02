#include "engine\editor\Editor.h"

#include "engine\editor\GPreferences.h"

#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\model\MModelObj.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\common.hpp>

#include <direct.h>
#include <shlobj.h>
#include <iostream>
#include <condition_variable>
#include <iostream>

std::vector<Editor::Project*> Editor::m_projects;
CTabBar* Editor::m_projectTabs;
Editor::Project* Editor::m_cProj = 0;

std::string* Editor::m_dataString = 0;

Editor::EditorState Editor::m_editorState;
Editor::EditorMode Editor::m_editorMode;

Editor::Editor() {
	m_deltaUpdate = 0.f;
	m_lastUpdate = 0.f;
	m_matrixCopy = 0;
	m_editorState = EditorState::STARTING;
	MTexture::init();
	MModelObj::init();
	//Font::loadFont("Leelaw", "leelawui.ttf", 12);
	Font::loadFont("Body", "segoeui.ttf", 12);
	Font::loadFont("Header", "segoeui.ttf", 20);
	Font::loadFont("Bold", "segoeui.ttf", 14);
	Font::setFont("Body");
	GGui::init(GScreen::getGLFWWindow());
	GBuffer::init();
	MTool::init();
	MMesh::init();
	GPreferences::init();

	glfwSetWindowIcon(GScreen::getGLFWWindow(), 1, MTexture::getTexture("gui\\icon\\window\\Logo.png", true)->getGlfwImage());

	Gui::init();
	Gui::getContainer()->addComponent(EditorOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return true; })->setPriorityLayer(10);
	Gui::getContainer()->addComponent(FreshOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return false; });
	Gui::getContainer()->addComponent(ModelOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return false; });
	Gui::getContainer()->resize();
	m_projectTabs = (CTabBar*)Gui::getContainer()->findComponent("GUI_EDITOR\\GUI_TOP\\TAB_FILES");

	setEditorState(EditorState::RUNNING);
	setEditorMode(EditorMode::NONE);

	m_autosavePeriod = 15;
	/*
	m_autosaveThread = new std::thread([&]() {
		GLfloat lastSave = glfwGetTime();
		std::unique_lock<std::mutex> lock(m_autosaveMutex);
		std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
		while (!m_autosaveCv.wait_until(lock, time + std::chrono::seconds(m_autosavePeriod), [&]() {
			time = std::chrono::system_clock::now();
			return m_editorState == EditorState::STOPPING;
			})) {
			//getModel()->autosave();
		}
		});
		*/

	initGBuffer();
	initAABuffer();

	m_sunlightDirection = glm::normalize(glm::vec3(0.5f, 4, 2));
}
Editor::~Editor() {
	m_editorState = EditorState::STOPPING;
	m_autosaveCv.notify_all();
	if (m_autosaveThread) {
		if (m_autosaveThread->joinable()) m_autosaveThread->join();
		delete m_autosaveThread;
	}
	MTexture::terminate();
	MModelObj::terminate();
	Font::clean();
	EditorOverlay::terminate();
	GGui::terminate();
	GBuffer::terminate();
	MMesh::terminate();
	MTool::terminate();

	terminateGBuffer();
	terminateAABuffer();
}
bool Editor::initGBuffer() {
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	
	// Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// Color + Specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	Uint32 attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Logger::logError("Framebuffer not complete!");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader::useProgram("deferredShading");
	Shader::setInt("gPosition", 0);
	Shader::setInt("gNormal", 1);
	Shader::setInt("gAlbedoSpec", 2);

	return true;
}
bool Editor::initAABuffer() {
	glGenFramebuffers(1, &aaBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aaBuffer);

	// Position color buffer
	glGenTextures(1, &aaScreenTex);
	glBindTexture(GL_TEXTURE_2D, aaScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aaScreenTex, 0);

	// Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
	Uint32 attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glGenRenderbuffers(1, &rboAA);
	glBindRenderbuffer(GL_RENDERBUFFER, rboAA);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboAA);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Logger::logError("Framebuffer not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader::useProgram("ssaa");
	Shader::setInt("aaSamples", GPreferences::getSamples());
	Shader::setInt("screenTex", 0);

	return true;
}
void Editor::terminateGBuffer() {
	glDeleteFramebuffers(1, &gBuffer);
	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gAlbedoSpec);
	glDeleteRenderbuffers(1, &rboDepth);
}
void Editor::terminateAABuffer() {
	glDeleteFramebuffers(1, &aaBuffer);
	glDeleteTextures(1, &aaScreenTex);
	glDeleteRenderbuffers(1, &rboAA);
}

void Editor::renderSquare() {
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Editor::renderCube() {
	// initialize (if necessary)
	if (cubeVAO == 0) {
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Editor::copyMatrix() {
	Model* m_model = getModel();
	if (m_model == 0 || m_model->getEditMatrix()->getId() == -1) return;
	if (m_matrixCopy) delete m_matrixCopy;
	m_matrixCopy = new Matrix(*m_model->getMatrix(m_model->getEditMatrix()->getId()));
}

void Editor::cutMatrix() {
	Model* m_model = getModel();
	if (m_model == 0 || m_model->getEditMatrix()->getId() == -1) return;
	copyMatrix();
	m_model->deleteSelectedMatrices();
}

void Editor::pasteMatrix() {
	Model* m_model = getModel();
	if (m_model == 0) return;
	if (m_matrixCopy) {
		m_model->getEditMatrix()->setCommandListOpen(false);
		m_model->addMatrix(m_matrixCopy->getName(), Camera::getPosition() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)), m_matrixCopy->getSize());
		Matrix* _matrix = m_model->getMatrix(Sint16(m_model->getMatrixNames().size()) - 1);
		_matrix->setPosition(Camera::getFocus() - (glm::vec3(m_matrixCopy->getSize()) / glm::vec3(2)));
		MNewCommand* _cmd = new MNewCommand(_matrix, m_model->getMatrixList(), Uint16(m_model->getMatrixList().size()) - 1, m_model->getNameList());
		m_model->getEditMatrix()->addCommand(_cmd);
		for (Uint16 x = 0; x < m_matrixCopy->getSize().x; x++) {
			for (Uint16 y = 0; y < m_matrixCopy->getSize().y; y++) {
				for (Uint16 z = 0; z < m_matrixCopy->getSize().z; z++) {
					_matrix->setVoxel({ x, y, z }, m_matrixCopy->getVoxel({ x, y, z }));
				}
			}
		}
		m_model->selectMatrix(m_model->getMatrixList().size() - 1);
		m_model->getEditMatrix()->reset();
		m_model->getEditMatrix()->setCommandListOpen(true);
		m_model->getEditMatrix()->addCommand(new MNewCommand(m_model->getMatrix(Sint16(m_model->getMatrixList().size()) - 1), m_model->getMatrixList(), Uint16(m_model->getMatrixList().size()) - 1, m_model->getNameList()));
	}
}

bool Editor::attemptClose() {
	for (Project* p : m_projects) {
		if (p->editor->hasChanged()) {
			Gui::openDialog("CloseEditor.lua");
			Gui::topDialog()->addFunction([&]() { GScreen::setExitting(2); });
			return true;
		}
	}
	GScreen::setExitting(2);
	return true;
}

void Editor::setDataString(std::string* p_dataString) {
	m_dataString = p_dataString;
}

void Editor::setEditorState(EditorState p_state) { m_editorState = p_state; }
Editor::EditorState Editor::getEditorState() { return m_editorState; }

void Editor::setEditorMode(EditorMode p_mode) {
	switch (m_editorMode) {
	case NONE:
		Gui::getContainer()->findComponent("GUI_FRESH")->setVisibleFunction([]() { return false; });
		break;
	case ANIMATION:
		Gui::getContainer()->findComponent("GUI_ANIMATION")->setVisibleFunction([]() { return false; });
		break;
	case MODEL:
		Gui::getContainer()->findComponent("GUI_MODEL")->setVisibleFunction([]() { return false; });
		break;
	default:
		break;
	}
	m_editorMode = p_mode;
	switch (m_editorMode) {
	case NONE:
		Gui::getContainer()->findComponent("GUI_FRESH")->setVisibleFunction([]() { return true; });
		break;
	case ANIMATION:
		Gui::getContainer()->findComponent("GUI_ANIMATION")->setVisibleFunction([]() { return true; });
		break;
	case MODEL:
		Gui::getContainer()->findComponent("GUI_MODEL")->setVisibleFunction([]() { return true; });
		break;
	default:
		return;
	}
	//Camera::reset();
}
Editor::EditorMode Editor::getEditorMode() {
	return m_editorMode;
}

void Editor::newProject(Project* p_project) {
	m_projects.insert(m_projects.begin(), p_project);
	m_cProj = p_project;
	m_projectTabs->addItem(m_cProj->editor->getName(), m_cProj->editor->getPath());
	setProject(0);
}
void Editor::setProject(Sint32 p_index) {
	if (p_index >= 0 && p_index < (Sint32)m_projects.size()) {
		m_cProj = m_projects[p_index];
		setEditorMode(m_cProj->mode);
		switch (m_cProj->mode) {
		case EditorMode::MODEL:
			getModel()->activate();
			getModel()->updateMatrixList();
			break;
		case EditorMode::ANIMATION:

			break;
		default:

			break;
		}
		m_projectTabs->setSelectedItem(p_index);
	} else {
		m_cProj = 0;
		setEditorMode(EditorMode::NONE);
	}
}
void Editor::closeProject(Sint32 p_index) {
	m_projectTabs->removeTab(p_index);
	delete static_cast<Model*>(m_projects.at(p_index)->editor);
	m_projects.erase(m_projects.begin() + p_index);
	setProject(m_projectTabs->getTabCount() - 1);
}
void Editor::closeSelectedProject() {
	attemptCloseProject(m_projectTabs->getSelectedItem());
}
void Editor::attemptCloseProject(Sint32 p_index) {
	if (p_index >= 0 && p_index < (Sint32)m_projects.size()) {
		setProject(p_index);
		if (m_projects.at(p_index)->editor->hasChanged()) {
			Gui::openDialog("CloseTab.lua");
			Gui::topDialog()->addFunction([&]() {
				closeProject(m_projectTabs->getSelectedItem());
				});
			Gui::topDialog()->addFunction([&]() {
				if (fileSave())
					closeProject(m_projectTabs->getSelectedItem());
				});
		} else {
			closeProject(m_projectTabs->getSelectedItem());
		}
	}
}

void Editor::resize() {
	Gui::resize();

	terminateGBuffer();
	terminateAABuffer();

	initGBuffer();
	initAABuffer();
}

void Editor::dropFile(const char* path) {
	Logger::logQuiet(path);
	LFormat::ImportType formatType = LFormat::valid(path);
	TEMode* editor = 0;
	switch (formatType) {
	case LFormat::ImportType::CSM:
	case LFormat::ImportType::NVM:
	case LFormat::ImportType::QB:
	case LFormat::ImportType::QBCL:
	case LFormat::ImportType::VOX:
		editor = new Model();
		static_cast<Model*>(editor)->init();
		static_cast<Model*>(editor)->setDataString(m_dataString);
		static_cast<Model*>(editor)->loadOpen(path);
		newProject(new Project(EditorMode::MODEL, editor));
		break;
	case LFormat::ImportType::NVA:
		//editor = new Animation();
		//((Animation*)editor)->init();
		break;
	case LFormat::ImportType::PNG:
		if (isModel()) {
			getModel()->addImageMatrix(path);
		}
		break;
	default: return;
	}
}

void Editor::renderMouse() {
	std::string tooltip = GGui::getTooltip();
	if (tooltip != "") {
		Sint32 b1 = 5, b2 = b1 - 1;
		Vector2<Sint32> size = Font::getMessageWidth(tooltip);
		Vector2<Sint32> pos = Vector2<Sint32>(GGui::getTooltipPos().x + 16 + b2, GGui::getTooltipPos().y + 21 + b2);
		pos.x = std::fminf(GScreen::getScreenSize().x - size.x - b2, std::fmaxf(b2, pos.x));
		if (pos.y > GScreen::getScreenSize().y - size.y - b2 - 26) {
			pos.y = GScreen::getScreenSize().y - size.y - b2 - 56;
		} else {
			pos.y = std::fmaxf(b2, pos.y);
		}

		Shader::pushMatrixModel();
		Shader::translate(glm::vec3(pos.x, pos.y, 0));

		GBuffer::setTexture(0);
		// Fill
		GBuffer::setColor(Color(0.8f, 0.8f, 0.8f, GGui::getTooltipFade()));
		GBuffer::addVertexQuad(-b1, -b1);
		GBuffer::addVertexQuad(size.x + b1, -b1);
		GBuffer::addVertexQuad(size.x + b1, size.y + b1);
		GBuffer::addVertexQuad(-b1, size.y + b1);

		// Border
		GBuffer::setColor(Color(0.75f, 0.75f, 0.75f, GGui::getTooltipFade()));
		GBuffer::addVertexQuad(-b1, -b1);
		GBuffer::addVertexQuad(size.x + b1, -b1);
		GBuffer::addVertexQuad(size.x + b1, -b2);
		GBuffer::addVertexQuad(-b1, -b2);

		GBuffer::addVertexQuad(size.x + b2, -b1);
		GBuffer::addVertexQuad(size.x + b2, size.y + b1);
		GBuffer::addVertexQuad(size.x + b1, size.y + b1);
		GBuffer::addVertexQuad(size.x + b1, -b1);

		GBuffer::addVertexQuad(size.x + b1, size.y + b2);
		GBuffer::addVertexQuad(-b1, size.y + b2);
		GBuffer::addVertexQuad(-b1, size.y + b1);
		GBuffer::addVertexQuad(size.x + b1, size.y + b1);

		GBuffer::addVertexQuad(-b1, size.y + b1);
		GBuffer::addVertexQuad(-b1, -b1);
		GBuffer::addVertexQuad(-b2, -b1);
		GBuffer::addVertexQuad(-b2, size.y + b1);

		Font::setAlignment(Alignment::ALIGN_LEFT);
		GBuffer::setColor(Color(0.1f, 0.1f, 0.1f, GGui::getTooltipFade()));
		Font::print(tooltip, 0, Font::getHeight() / 2);
		Shader::popMatrixModel();
	}
}

void Editor::bindGBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::vec3 Editor::getSunlightDir() {
	glm::vec4 lRayStart_NDC(0.f, 0.f, -1.0f, 1.0f);
	glm::vec4 lRayEnd_NDC(0.f, 0.f, 0.f, 1.0f);

	glm::mat4 iProjectionMatrix = glm::inverse(Shader::getMatrixProjection());
	glm::mat4 iViewMatrix = glm::inverse(getSunlightMatrix());

	glm::vec4 lRayStart_camera = iProjectionMatrix * lRayStart_NDC;	lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = iViewMatrix * lRayStart_camera;		lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = iProjectionMatrix * lRayEnd_NDC;		lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = iViewMatrix * lRayEnd_camera;			lRayEnd_world /= lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	return glm::normalize(lRayDir_world) * glm::vec3(-1, -1, -1);
}
glm::mat4 Editor::getSunlightMatrix() {
	Shader::transformView(glm::mat4());
	glm::mat4 lightMatrix(1.f);
	lightMatrix = glm::rotate(lightMatrix, glm::radians(45.f), glm::vec3(1.0f, 0.0f, 0.0f));
	lightMatrix = glm::rotate(lightMatrix, glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
	return lightMatrix;
}

void Editor::input() {
	GFormat::checkLoadFunction();

	Sint8 _iFlags = (Sint8)Component::EventFlag::ALL;
	if (!GMouse::isMouseActive())
		_iFlags -= static_cast<Sint8>(Component::EventFlag::MOUSEOVER);
	Gui::input(_iFlags);

	if (m_cProj && !m_cProj->editor->isBuilding()) {
		m_cProj->editor->input(_iFlags);
		m_projectTabs->setSelectedState(m_projectTabs->getSelectedItem(), m_cProj->editor->hasChanged());
	}

	if (GKey::keyPressed(GLFW_KEY_O)) {
		MTexture::reload();
		Component::loadTheme();
	}
}

void Editor::update() {
	m_deltaUpdate = std::fminf(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
	m_lastUpdate = GLfloat(glfwGetTime());

	GScreen::setDeltaUpdate(m_deltaUpdate);

	if (m_cProj && !m_cProj->editor->isBuilding()) {
		m_cProj->editor->update(m_deltaUpdate);
	}

	Gui::update(m_deltaUpdate);
}

void Editor::render2d() {
	GBuffer::clean();
	Font::setAlignment(Alignment::ALIGN_LEFT);
	Gui::render();
	renderMouse();
	GBuffer::rasterize();
	GBuffer::renderMesh();
}
void Editor::renderGeometry() {
	Font::setFont("Body");
	glViewport(0, 0, GScreen::getScreenSize().x * GPreferences::getSamples(), GScreen::getScreenSize().y * GPreferences::getSamples());
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::useProgram("gBuffer");
	Shader::applyProjection();
	Shader::applyView();
	if (m_cProj && !m_cProj->editor->isBuilding()) {
		m_cProj->editor->render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Editor::renderLight() {
	glBindFramebuffer(GL_FRAMEBUFFER, aaBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::useProgram("deferredShading");
	Shader::setTexture(0, gPosition);
	Shader::setTexture(1, gNormal);
	Shader::setTexture(2, gAlbedoSpec);
	Shader::setVec3("viewPos", Camera::getPosition());
	Camera::applyLightDirection();
	
	renderSquare();

	glViewport(0, 0, GScreen::getScreenSize().x, GScreen::getScreenSize().y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Shader::useProgram("ssaa");
	Shader::setTexture(0, aaScreenTex);
	renderSquare();
}

bool Editor::fileNewModel() {
	EditorMode mode = EditorMode::MODEL;
	Model* editor = new Model();
	editor->init();
	editor->setDataString(m_dataString);
	newProject(new Project(mode, editor));
	editor->setBuilding(false);
	return true;
}
bool Editor::fileOpen() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Chronovox Studio");
	if (_mkdir(documents)) {
		Logger::logDiagnostic("Directory made");
	}

	char filename[MAX_PATH] = {};
	OPENFILENAME ofn;

	memset(&filename, 0, sizeof(filename));
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Model (*.csm, *.nvm, *.qb, *.png)\0"
		"*.csm;*.nvm;*.qb;*.png*\0"
		//"Animation (*.nva)\0"
		//"*.nva\0"
		"Any File\0"
		"*.*\0\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Model";
	if (!GetOpenFileNameA(&ofn)) return false;

	LFormat::ImportType formatType;
	EditorMode mode;
	TEMode* editor;
	formatType = LFormat::valid(filename);
	switch (formatType) {
	case LFormat::ImportType::CSM:
	case LFormat::ImportType::NVM:
	case LFormat::ImportType::QB:
	case LFormat::ImportType::QBCL:
	case LFormat::ImportType::VOX:
		mode = EditorMode::MODEL;
		editor = new Model();
		((Model*)editor)->init();
		((Model*)editor)->loadOpen(filename);
		((Model*)editor)->setDataString(m_dataString);
		break;
	default: return false;
	}

	newProject(new Project(mode, editor));

	return true;
}
bool Editor::fileSave() {
	if (m_cProj && !m_cProj->editor->isBuilding()) {
		bool s = m_cProj->editor->fileSave();
		if (s)
			m_projectTabs->renameItem(m_projectTabs->getSelectedItem(), m_cProj->editor->getName(), m_cProj->editor->getPath());
		return s;
	}
	return false;
}
bool Editor::fileSaveAs() {
	if (m_cProj && !m_cProj->editor->isBuilding()) {
		bool s = m_cProj->editor->fileSaveAs();
		if (s)
			m_projectTabs->renameItem(m_projectTabs->getSelectedItem(), m_cProj->editor->getName(), m_cProj->editor->getPath());
		return s;
	}
	return false;
}
bool Editor::fileExit() {
	GScreen::setExitting(1);
	//if (m_tMode) m_tMode->fileExit();
	return true;
}

void Editor::editUndo() {
	//if (m_tMode) m_tMode->editUndo();
}
void Editor::editRedo() {
	//if (m_tMode) m_tMode->editRedo();
}
void Editor::editPreferences() {
	Gui::openDialog("Preferences.lua");
	Gui::topDialog()->findComponent("FFPS")->setValue(GPreferences::getFocusFPS());
	Gui::topDialog()->findComponent("UFPS")->setValue(GPreferences::getUnfocusFPS());
	Gui::topDialog()->findComponent("VIEW")->setSelectedItem(static_cast<Sint32>(GPreferences::getViewMode()));
	Gui::topDialog()->findComponent("AA")->setSelectedItem(static_cast<Sint32>(GPreferences::getAntiAlias()));
	Gui::topDialog()->findComponent("NGRID")->setValue(GPreferences::getGridCount());
	Gui::topDialog()->findComponent("SGRID")->setValue(GPreferences::getGridSpace());
	Gui::topDialog()->findComponent("AUTORESIZE")->setSelectedItem(static_cast<Sint32>(GPreferences::getAutoResize()));
	Gui::topDialog()->addFunction([&]() {
		GPreferences::setFocusFPS(Gui::topDialog()->findComponent("FFPS")->getValue());
		GPreferences::setUnfocusFPS(Gui::topDialog()->findComponent("UFPS")->getValue());
		GPreferences::setViewMode(static_cast<GPreferences::ViewMode>(Gui::topDialog()->findComponent("VIEW")->getSelectedItem()));
		GPreferences::setAntiAlias(static_cast<GPreferences::AntiAlias>(Gui::topDialog()->findComponent("AA")->getSelectedItem()));
		GPreferences::setGridCount(Gui::topDialog()->findComponent("NGRID")->getValue());
		GPreferences::setGridSpace(Gui::topDialog()->findComponent("SGRID")->getValue());
		GPreferences::setAutoResize(static_cast<GPreferences::AutoResize>(Gui::topDialog()->findComponent("AUTORESIZE")->getSelectedItem()));
		GPreferences::save();
		resize();
		});
}

void Editor::helpAbout() {
	Gui::openDialog("About.lua");
	static_cast<CTextFile*>(Gui::topDialog()->findComponent("ABOUT"))->addInputString(GScreen::getAppVersion());
	Gui::topDialog()->addFunction([]() { ShellExecute(0, 0, "www.nickvoxel.com/chronovox-studio", 0, 0, SW_SHOW); });
}
