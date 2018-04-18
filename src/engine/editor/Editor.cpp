#include "engine\editor\Editor.h"

#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"

#include "engine\sfx\Sound.h"

#include <iostream>

Animation* Editor::m_animation = 0;
Model* Editor::m_model = 0;
TEMode* Editor::m_tMode = 0;

Editor::EditorState Editor::m_editorState;
Editor::EditorMode Editor::m_editorMode;

Container* Editor::m_mainGui;

Editor::Editor() {
	m_editorState = EditorState::STARTING;
	MTexture::init();
	Font::loadFont("LeelawUI", "res\\font\\leelawui.ttf", 10);
	Font::loadFont("SegoeUI", "res\\font\\segoeui.ttf", 10);
	Sound::getInstance().init();
	GGui::init(GScreen::m_window);
	m_animation = new Animation();
	m_model = new Model();
	m_mainGui = new Container();
	m_mainGui = EditorOverlay::init(this);
	m_mainGui->addComponent(ModelOverlay::init(m_model), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_mainGui->addComponent(AnimationOverlay::init(m_animation), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_animation->init(m_model);
	m_model->init();
	m_autosaveThread = new std::thread([this]() {
		GLfloat lastSave = glfwGetTime();
		while(m_editorState != EditorState::CLOSING) {
			if(glfwGetTime() - lastSave > 2) {
				getModel()->autosave();
				lastSave = glfwGetTime();
			}
			Sleep(100);
		}
		getModel()->autosave();
	});
	setEditorState(EditorState::RUNNING);
	setEditorMode(EditorMode::MODEL);
}
Editor::~Editor() {
	m_editorState = EditorState::CLOSING;
	if(m_autosaveThread->joinable()) m_autosaveThread->join();
	delete m_autosaveThread;
	MTexture::terminate();
	Sound::getInstance().terminate();
	Font::clean();
	EditorOverlay::terminate();
	delete m_model;
}
bool Editor::attemptClose() {
	//return getModel()->exitSave();
	getModel()->autosave();
	return true;
}

void Editor::setEditorState(EditorState p_state) { m_editorState = p_state; }
Editor::EditorState Editor::getEditorState() { return m_editorState; }

void Editor::setEditorMode(EditorMode p_mode) {
	switch(m_editorMode) {
	case ANIMATION:
		m_mainGui->findComponent("GUI_ANIMATION")->setVisible(false);
		break;
	case MODEL:
		m_mainGui->findComponent("GUI_MODEL")->setVisible(false);
		break;
	default:
		break;
	}
	m_editorMode = p_mode;
	switch(m_editorMode) {
	case ANIMATION:
		m_tMode = m_animation;
		m_mainGui->findComponent("GUI_ANIMATION")->setVisible(true);
		break;
	case MODEL:
		m_tMode = m_model;
		m_mainGui->findComponent("GUI_MODEL")->setVisible(true);
		break;
	default:
		m_tMode = 0;
		return;
	}
	Camera::reset();
}
Editor::EditorMode Editor::getEditorMode() {
	return m_editorMode;
}

void Editor::resize() {
	m_mainGui->setSize(GScreen::m_screenSize);
	m_mainGui->resize();
}

void Editor::dropFile(const char* path) {
	m_model->load(path);
}

void Editor::renderMouse() {
	std::string tooltip = GGui::getTooltip();
	if(tooltip != "") {
		Sint32 b1 = 10, b2 = b1 + 1;
		Vector2<Sint32> size = Font::getMessageWidth(tooltip);
		Vector2<Sint32> pos = Vector2<Sint32>(GGui::getTooltipPos().x + b2, GGui::getTooltipPos().y + 23 + b2);
		pos.x = min(GScreen::m_screenSize.x - size.x - b2, max(b2, pos.x));
		pos.y = min(GScreen::m_screenSize.y - size.y - b2, max(b2, pos.y));
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glTranslatef(pos.x, pos.y, 0);
			glColor3f(0.3f, 0.3f, 0.3f);
			glBegin(GL_QUADS);
			{
				glVertex2f(-b1,				-b1);
				glVertex2f(size.x + b1,		-b1);
				glVertex2f(size.x + b1,		size.y + b1);
				glVertex2f(-b1,				size.y + b1);
			}
			glEnd();
			glColor3f(0.35f, 0.35f, 0.35f);
			glBegin(GL_QUADS);
			{
				glVertex2f(-b1,				-b1);
				glVertex2f(size.x + b1,		-b1);
				glVertex2f(size.x + b1,		-b2);
				glVertex2f(-b1,				-b2);

				glVertex2f(size.x + b2,		-b1);
				glVertex2f(size.x + b2,		size.y + b1);
				glVertex2f(size.x + b1,		size.y + b1);
				glVertex2f(size.x + b1,		-b1);

				glVertex2f(size.x + b1,		size.y + b2);
				glVertex2f(-b1,				size.y + b2);
				glVertex2f(-b1,				size.y + b1);
				glVertex2f(size.x + b1,		size.y + b1);

				glVertex2f(-b1,				size.y + b1);
				glVertex2f(-b1,				-b1);
				glVertex2f(-b2,				-b1);
				glVertex2f(-b2,				size.y + b1);
			}
			glEnd();
			Font::setAlignment(Alignment::ALIGN_LEFT);
			glColor3f(1, 1, 1);
			Font::print(tooltip, 0, Font::getHeight() / 2);
		}
		glPopMatrix();
	}
}

void Editor::input() {
	Sint8 _interact = (Sint8)Component::EventFlag::ALL;

	m_mainGui->input(_interact);

	switch(m_editorMode) {
	case EditorMode::ANIMATION:
		m_animation->input(_interact);
		break;
	case EditorMode::MODEL:
		m_model->input(_interact);
		break;
	default: break;
	}
	if(GKey::keyPressed(GLFW_KEY_F4, GLFW_MOD_ALT)) GScreen::m_exitting = 2;
}

void Editor::update() {
	m_deltaUpdate = min(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
	m_lastUpdate = GLfloat(glfwGetTime());

	GScreen::m_deltaTime = m_deltaUpdate;

	switch(m_editorMode) {
	case EditorMode::ANIMATION:
		m_animation->update(m_deltaUpdate);
		break;
	case EditorMode::MODEL:
		m_model->update(m_deltaUpdate);
		break;
	default: break;
	}

	m_mainGui->update(m_deltaUpdate);
	GGui::update();
}

void Editor::render3d() {
	switch(m_editorMode) {
	case EditorMode::ANIMATION:
		m_animation->render();
		break;
	case EditorMode::MODEL:
		m_model->render();
		break;
	default: break;
	}
}
void Editor::render2d() {
	Font::setAlignment(Alignment::ALIGN_LEFT);
	m_mainGui->render();
	renderMouse();
}

void Editor::fileNew() {
	if(m_tMode) m_tMode->fileNew();
}
void Editor::fileOpen() {
	if(m_tMode) m_tMode->fileOpen();
}
void Editor::fileSave() {
	if(m_tMode) m_tMode->fileSave();
}
void Editor::fileExit() {
	GScreen::m_exitting = 1;
	if(m_tMode) m_tMode->fileExit();
}

void Editor::editUndo() {
	if(m_tMode) m_tMode->editUndo();
}
void Editor::editRedo() {
	if(m_tMode) m_tMode->editRedo();
}
