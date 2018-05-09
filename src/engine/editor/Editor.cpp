#include "engine\editor\Editor.h"

#include "engine\editor\menu\EditorOverlay.h"
#include "engine\editor\animation\menu\AnimationOverlay.h"
#include "engine\editor\model\menu\ModelOverlay.h"

#include "engine\gfx\font\Font.h"
#include "engine\gfx\texture\MTexture.h"
#include "engine\gfx\model\MModelObj.h"
#include "engine\gfx\shader\Shader.h"

#include "engine\sfx\Sound.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\common.hpp>

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
	MModelObj::init();
	Font::loadFont("LeelawUI", "res\\font\\leelawui.ttf", 10);
	Font::loadFont("SegoeUI", "res\\font\\segoeui.ttf", 10);
	Sound::getInstance().init();
	GGui::init(GScreen::m_window);
	m_animation = new Animation();
	m_model = new Model();
	m_mainGui = EditorOverlay::init(this);
	m_mainGui->addComponent(ModelOverlay::init(m_model), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_mainGui->addComponent(AnimationOverlay::init(m_animation), Component::Anchor::NONE, Component::Anchor::BOTTOM_RIGHT)->setVisible(false);
	m_animation->init(m_model);
	m_model->init();
	m_autosaveThread = new std::thread([&]() {
		GLfloat lastSave = glfwGetTime();
		while(m_editorState != EditorState::CLOSING) {
			if(glfwGetTime() - lastSave > 2) {
				getModel()->autosave();
				lastSave = glfwGetTime();
			}
			Sleep(1000);
		}
		getModel()->autosave();
	});
	setEditorState(EditorState::RUNNING);
	setEditorMode(EditorMode::MODEL);

	initShadowBuffer();

	m_sunlightDirection = glm::normalize(glm::vec3(0.5f, 4, 2));
}
Editor::~Editor() {
	m_editorState = EditorState::CLOSING;
	if(m_autosaveThread->joinable()) m_autosaveThread->join();
	delete m_autosaveThread;
	MTexture::terminate();
	MModelObj::terminate();
	Sound::getInstance().terminate();
	Font::clean();
	EditorOverlay::terminate();
	GGui::terminate();
	delete m_model;

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
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
void Editor::terminateShadowBuffer() {
	glDeleteFramebuffers(1, &m_shadowBuffer.framebufferName);
	glDeleteTextures(1, &m_shadowBuffer.renderedTexture);
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
		pos.x = std::fminf(GScreen::m_screenSize.x - size.x - b2, std::fmaxf(b2, pos.x));
		pos.y = std::fminf(GScreen::m_screenSize.y - size.y - b2, std::fmaxf(b2, pos.y));
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
	glViewport(0, 0, GScreen::m_screenSize.x, GScreen::m_screenSize.y);
}
void Editor::renderShadowTexture() {
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(1, 0); glVertex2f(GScreen::m_screenSize.x, 0);
		glTexCoord2f(1, 1); glVertex2f(GScreen::m_screenSize.x, GScreen::m_screenSize.y);
		glTexCoord2f(0, 1); glVertex2f(0, GScreen::m_screenSize.y);
	}
	glEnd();
}

glm::vec3 Editor::getSunlightDir() {
	glm::vec4 lRayStart_NDC(0.f, 0.f, -1.0f, 1.0f);
	glm::vec4 lRayEnd_NDC  (0.f, 0.f,  0.f,  1.0f);

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
	m_deltaUpdate = std::fminf(0.5f, GLfloat(glfwGetTime() - m_lastUpdate));
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

void Editor::renderShadow() {
	switch(m_editorMode) {
	case EditorMode::ANIMATION:
		m_animation->renderShadow();
		break;
	case EditorMode::MODEL:
		m_model->renderShadow();
		break;
	default: break;
	}
}
void Editor::render3d() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.renderedTexture);
	glUniform1i(7, 1);
	glActiveTexture(GL_TEXTURE0);
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
