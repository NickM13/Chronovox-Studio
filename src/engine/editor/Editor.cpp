#include "engine\editor\Editor.h"

#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\menu\FreshOverlay.h"
#include "engine\editor\menu\CloseEditorDialog.h"
#include "engine\editor\menu\CloseTabDialog.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\model\MModelObj.h"

#include "engine\sfx\Sound.h"

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
	m_editorState = EditorState::STARTING;
	MTexture::init();
	MModelObj::init();
	Font::loadFont("Leelaw", "leelawui.ttf", 12);
	Font::loadFont("Segoe", "segoeui.ttf", 12);
	Font::setFont("Segoe");
	Sound::getInstance().init();
	GGui::init(GScreen::getGLFWWindow());
	GBuffer::init();
	MTool::init();
	MMesh::init();

	glfwSetWindowIcon(GScreen::getGLFWWindow(), 1, MTexture::getTexture("gui\\icon\\window\\Logo.png")->getGlfwImage());

	Gui::init();
	Gui::getContainer()->addComponent(EditorOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return true; })->setPriorityLayer(10);
	Gui::getContainer()->addComponent(FreshOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return false; });
	Gui::getContainer()->addComponent(ModelOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return false; });
	Gui::getContainer()->addComponent(AnimationOverlay::init(this), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)
		->setVisibleFunction([]() { return false; });
	Gui::getContainer()->resize();
	m_projectTabs = (CTabBar*)Gui::getContainer()->findComponent("GUI_EDITOR\\GUI_TOP\\TAB_FILES");

	setEditorState(EditorState::RUNNING);
	setEditorMode(EditorMode::NONE);

	m_autosavePeriod = 15;
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

	initShadowBuffer();

	m_sunlightDirection = glm::normalize(glm::vec3(0.5f, 4, 2));
}
Editor::~Editor() {
	m_editorState = EditorState::STOPPING;
	m_autosaveCv.notify_all();
	if (m_autosaveThread->joinable()) m_autosaveThread->join();
	delete m_autosaveThread;
	MTexture::terminate();
	MModelObj::terminate();
	Sound::getInstance().terminate();
	Font::clean();
	EditorOverlay::terminate();
	GGui::terminate();
	GBuffer::terminate();
	MMesh::terminate();
	MTool::terminate();

	terminateShadowBuffer();
}
bool Editor::initShadowBuffer() {
	m_shadowBuffer.framebufferName = 0;
	glGenFramebuffers(1, &m_shadowBuffer.framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer.framebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	m_shadowBuffer.shadowSize = glm::vec2(4096, 4096);
	glGenTextures(1, &m_shadowBuffer.renderedTexture);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_shadowBuffer.shadowSize.x, m_shadowBuffer.shadowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowBuffer.renderedTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

						   // Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
void Editor::terminateShadowBuffer() {
	glDeleteFramebuffers(1, &m_shadowBuffer.framebufferName);
	glDeleteTextures(1, &m_shadowBuffer.renderedTexture);
}
bool Editor::attemptClose() {
	for (Project* p : m_projects) {
		if (p->editor->hasChanged()) {
			Gui::openDialog(CloseEditorDialog::getInstance().getDialog()
				->setOptionFunc("Exit Without Saving", [&]() {
					GScreen::setWindowCommand(GScreen::WindowCommand::CLOSE);
					CloseEditorDialog::getInstance().getDialog()->setActive(false);
					}));
			return true;
		}
	}
	GScreen::setWindowCommand(GScreen::WindowCommand::CLOSE);
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
			Gui::openDialog(CloseTabDialog::getInstance().getDialog()
				->setOptionFunc("Save", [&]() {
					if (fileSave()) {
						closeProject(m_projectTabs->getSelectedItem());
						CloseTabDialog::getInstance().getDialog()->setActive(false);
					}
					})->setOptionFunc("Don't Save", [&]() {
						closeProject(m_projectTabs->getSelectedItem());
						CloseTabDialog::getInstance().getDialog()->setActive(false);
						}));
		} else {
			closeProject(m_projectTabs->getSelectedItem());
		}
	}
}

void Editor::resize() {
	Gui::resize();
}

void Editor::dropFile(const char* path) {
	Format::FormatType formatType = Format::valid(path);
	EditorMode mode;
	TEMode* editor;
	switch (formatType) {
	case Format::FormatType::CSM:
	case Format::FormatType::NVM:
	case Format::FormatType::QB:
	case Format::FormatType::QBCL:
	case Format::FormatType::VOX:
		mode = EditorMode::MODEL;
		editor = new Model();
		((Model*)editor)->init();
		((Model*)editor)->loadOpen(path);
		((Model*)editor)->setDataString(m_dataString);
		break;
	case Format::FormatType::NVA:
		mode = EditorMode::ANIMATION;
		editor = new Animation();
		//((Animation*)editor)->init();
		break;
	default: return;
	}

	newProject(new Project(mode, editor));
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

void Editor::bindShadowBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer.framebufferName);
	glViewport(0, 0, m_shadowBuffer.shadowSize.x, m_shadowBuffer.shadowSize.y);
}
void Editor::bindShadowTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);

}
void Editor::unbindShadowBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GScreen::getScreenSize().x, GScreen::getScreenSize().y);
}
void Editor::renderShadowTexture() {
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(GScreen::getScreenSize().x, 0);
		glTexCoord2f(1, 1); glVertex2f(GScreen::getScreenSize().x, GScreen::getScreenSize().y);
		glTexCoord2f(0, 1); glVertex2f(0, GScreen::getScreenSize().y);
	}
	glEnd();
}

glm::vec3 Editor::getSunlightDir() {
	glm::vec4 lRayStart_NDC(0.f, 0.f, -1.0f, 1.0f);
	glm::vec4 lRayEnd_NDC(0.f, 0.f, 0.f, 1.0f);

	glm::mat4 iProjectionMatrix = glm::inverse(Shader::getMatrixProjection());
	glm::mat4 iViewMatrix = glm::inverse(getSunlightMatrix());

	glm::vec4 lRayStart_camera = iProjectionMatrix * lRayStart_NDC;      lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = iViewMatrix * lRayStart_camera;   lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = iProjectionMatrix * lRayEnd_NDC;	     lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = iViewMatrix * lRayEnd_camera;     lRayEnd_world /= lRayEnd_world.w;

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
	Sint8 _iFlags = (Sint8)Component::EventFlag::ALL;
	if (!GMouse::isMouseActive())
		_iFlags -= static_cast<Sint8>(Component::EventFlag::MOUSEOVER);
	Gui::input(_iFlags);

	if (m_cProj) {
		m_cProj->editor->input(_iFlags);
		m_projectTabs->setSelectedState(m_projectTabs->getSelectedItem(), m_cProj->editor->hasChanged());
	}

	if (GKey::keyPressed(GLFW_KEY_O)) {
		MTexture::reload();
		Component::loadTheme();
	}
	//if (GKey::keyPressed(GLFW_KEY_F4, GLFW_MOD_ALT)) GScreen::m_exitting = 2;
}

void Editor::update() {
	m_deltaUpdate = std::fminf(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
	m_lastUpdate = GLfloat(glfwGetTime());

	GScreen::setDeltaUpdate(m_deltaUpdate);

	if (m_cProj) {
		m_cProj->editor->update(m_deltaUpdate);
	}

	Gui::update(m_deltaUpdate);
}

void Editor::renderShadow() {
	if (m_cProj) {
		m_cProj->editor->renderShadow();
	}
}
void Editor::render3d() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);
	glUniform1i(7, 1);
	glActiveTexture(GL_TEXTURE0);
	if (m_cProj) {
		m_cProj->editor->render();
	}
}
void Editor::render2d() {
	GBuffer::clean();
	Font::setAlignment(Alignment::ALIGN_LEFT);
	Gui::render();
	renderMouse();
	GBuffer::rasterize();
	GBuffer::renderMesh();
}

bool Editor::fileNewModel() {
	EditorMode mode = EditorMode::MODEL;
	Model* editor = new Model();
	editor->init();
	editor->setDataString(m_dataString);
	newProject(new Project(mode, editor));
	return true;
}
bool Editor::fileNewAnimation() {
	EditorMode mode = EditorMode::ANIMATION;
	Animation* editor = new Animation();
	SimpleModel* testModel = new SimpleModel();
	editor->init(testModel);
	newProject(new Project(mode, editor));
	return true;
}
bool Editor::fileOpen() {
	char documents[MAX_PATH];
	HRESULT res = SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\Voxel Models");
	if (_mkdir(documents)) {
		Logger::logDiagnostic("Directory made");
	}

	char filename[MAX_PATH] = {};
	OPENFILENAME ofn;

	memset(&filename, 0, sizeof(filename));
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Model (*.csm, *.nvm, *.qb)\0"
		"*.csm;*.nvm;*.qb*\0"
		//"Animation (*.nva)\0"
		//"*.nva\0"
		"Any File\0"
		"*.*\0\0";
	ofn.lpstrFile = filename;
	if (res == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Model";
	if (!GetOpenFileNameA(&ofn)) return false;

	Format::FormatType formatType;
	EditorMode mode;
	TEMode* editor;
	formatType = Format::valid(filename);
	switch (formatType) {
	case Format::FormatType::CSM:
	case Format::FormatType::NVM:
	case Format::FormatType::QB:
	case Format::FormatType::QBCL:
	case Format::FormatType::VOX:
		mode = EditorMode::MODEL;
		editor = new Model();
		((Model*)editor)->init();
		((Model*)editor)->loadOpen(filename);
		((Model*)editor)->setDataString(m_dataString);
		break;
	case Format::FormatType::NVA:
		mode = EditorMode::ANIMATION;
		editor = new Animation();
		//((Animation*)editor)->init();
		break;
	default: return false;
	}

	newProject(new Project(mode, editor));

	return true;
}
bool Editor::fileSave() {
	if (m_cProj) {
		bool s = m_cProj->editor->fileSave();
		if (s)
			m_projectTabs->renameItem(m_projectTabs->getSelectedItem(), m_cProj->editor->getName(), m_cProj->editor->getPath());
		return s;
	}
	return false;
}
bool Editor::fileSaveAs() {
	if (m_cProj) {
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
