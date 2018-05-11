#include "engine\editor\camera\Camera.h"
#include "engine\editor\menu\EditorOverlay.h"
#include "engine\gfx\shader\Shader.h"
#include "engine\gfx\model\MModelObj.h"
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtx\perpendicular.hpp>
#include <glm\gtx\normal.hpp>

glm::vec3 Camera::m_position, Camera::m_rotation;
GLfloat Camera::m_zoom, Camera::m_tarZoom, Camera::m_zoomSpeed;
Texture* Camera::m_skyTexture;

void Camera::init() {
	m_skyTexture = MTexture::getTexture("DaylightSky.png");
	reset();
}
void Camera::reset() {
	m_position = {0, 6, 0};
	m_rotation = {30, 30, 0};
	m_zoom = 32;
	m_tarZoom = 0;
	m_zoomSpeed = 10.f;
}

void Camera::zoom(GLfloat p_scroll) {
	m_tarZoom += p_scroll;
	if(m_zoom - m_tarZoom < 0)
		m_tarZoom = m_zoom;
	else if(m_zoom - m_tarZoom > 256)
		m_tarZoom = m_zoom - 256;
}
void Camera::turn(Vector2<Sint32> p_mouseMove) {
	m_rotation = m_rotation + glm::vec3(GLfloat(p_mouseMove.y), GLfloat(p_mouseMove.x), 0) * 0.5f;

	if(m_rotation.x < -90) m_rotation.x = -90;
	if(m_rotation.x > 90) m_rotation.x = 90;

	m_rotation.y = fmodf(m_rotation.y, 360);
}
void Camera::pan(Vector3<GLfloat> p_panDir) {
	GLfloat _sensitivity = m_zoom * 0.0015f + 0.0135f;
	glm::vec3 dir = getDirection();
	dir = glm::perp(dir, glm::vec3(0, 1, 0));
	dir = glm::vec3(getViewMatrix()[0][0], getViewMatrix()[1][0], getViewMatrix()[2][0]);
	m_position = m_position + dir * p_panDir.x * _sensitivity;
	dir = glm::vec3(getViewMatrix()[0][1], getViewMatrix()[1][1], getViewMatrix()[2][1]);
	m_position = m_position + dir * p_panDir.y * _sensitivity;
	dir = glm::vec3(getViewMatrix()[0][2], getViewMatrix()[1][2], getViewMatrix()[2][2]);
	m_position = m_position + dir * p_panDir.z * _sensitivity;
}

void Camera::setPosition(glm::vec3 p_position) {
	m_position = p_position;
}

glm::vec3 Camera::getFocus() {
	return m_position;
}
glm::vec3 Camera::getPosition() {
	glm::mat4 matrixView = glm::inverse(getViewMatrix());
	return matrixView[3];
}
glm::vec3 Camera::getDirection() {
	glm::mat4 viewMatrix = getViewMatrix();
	return glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));
}
glm::vec3 Camera::getRotation() {
	return m_rotation;
}
glm::vec3 Camera::getMouseDirection() {
	glm::vec4 lRayStart_NDC(
		((GLfloat)GMouse::getMousePos().x / (GLfloat)GScreen::m_screenSize.x - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((GLfloat)GMouse::getMousePos().y / (GLfloat)GScreen::m_screenSize.y - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((GLfloat)GMouse::getMousePos().x / (GLfloat)GScreen::m_screenSize.x - 0.5f) * 2.0f,
		((GLfloat)GMouse::getMousePos().y / (GLfloat)GScreen::m_screenSize.y - 0.5f) * -2.0f,
		0.0,
		1.0f
	);

	glm::mat4 iProjectionMatrix = glm::inverse(Shader::getMatrixProjection());
	glm::mat4 iViewMatrix = glm::inverse(Camera::getViewMatrix());

	glm::vec4 lRayStart_camera = iProjectionMatrix * lRayStart_NDC;      lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = iViewMatrix * lRayStart_camera;   lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = iProjectionMatrix * lRayEnd_NDC;	     lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = iViewMatrix * lRayEnd_camera;     lRayEnd_world /= lRayEnd_world.w;

	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	return glm::normalize(lRayDir_world);
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.f, 0.f, -m_zoom));
	view = glm::rotate(view, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	view = glm::rotate(view, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	view = glm::translate(view, glm::vec3(-m_position.x, -m_position.y, -m_position.z));
	return view;
}
glm::mat4 Camera::getModelViewProjection() {
	glm::mat4 projection = glm::perspective((GLfloat)glm::radians(70.f), (GLfloat)GScreen::m_screenSize.x / GScreen::m_screenSize.y, 0.1f, 100.0f);
	glm::mat4 view = getViewMatrix();
	return projection * getViewMatrix();
}

void Camera::applyLightDirection() {
	Shader::setLightDirection(getDirection());
}
void Camera::applyTransformation() {
	Shader::transformView(getViewMatrix());
	Shader::applyView();
}

void Camera::input(Sint8 p_guiFlags) {
	if(!EditorOverlay::getContainer()->isPaused() && (p_guiFlags & (Sint8)Component::EventFlag::MOUSEOVER)) {
		if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_SHIFT))
			pan(Vector3<GLfloat>(-GMouse::getDeltaMousePos().x, GMouse::getDeltaMousePos().y, 0.f));
		else if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT))
			turn(GMouse::getDeltaMousePos());
		zoom(GMouse::getMouseScroll());
		GLfloat speed = 4;
		if(GKey::keyDown(GLFW_KEY_D, 0)) pan({speed,  0,      0});
		if(GKey::keyDown(GLFW_KEY_A, 0)) pan({-speed,  0,      0});
		if(GKey::keyDown(GLFW_KEY_Q, 0)) pan({0,      speed,  0});
		if(GKey::keyDown(GLFW_KEY_Z, 0)) pan({0,     -speed,  0});
		if(GKey::keyDown(GLFW_KEY_W, 0)) pan({0,      0,     -speed});
		if(GKey::keyDown(GLFW_KEY_S, 0)) pan({0,      0,      speed});
	}
}

void Camera::update(GLfloat p_deltaUpdate) {
	GLfloat _zoom = m_tarZoom * m_zoomSpeed * p_deltaUpdate;
	m_zoom -= _zoom;
	m_tarZoom -= _zoom;
}

void Camera::renderSkybox() {
	Shader::pushMatrixModel();
	glm::mat4 t = glm::inverse(Camera::getViewMatrix());
	t = glm::rotate(t, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	t = glm::rotate(t, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	t = glm::rotate(t, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	t = glm::scale(t, glm::vec3(500, 500, 500));
	Shader::transformModel(t);
	Shader::setColor(glm::vec4(1, 1, 1, 1));
	Shader::setTexture(0, "DaylightSky.png");
	MModelObj::get("Skybox.obj")->renderObj();
	Shader::setTexture(0, 0);
	Shader::popMatrixModel();
}
void Camera::renderFocus() {
	glm::vec3 _size = glm::vec3(0.125f);
	Shader::pushMatrixModel();
	Shader::transformModel(glm::translate(glm::mat4(), m_position));
	Shader::transformModel(glm::scale(glm::mat4(), _size));
	Shader::setColor(glm::vec4(1, 1, 0, 0.5f));
	MModelObj::get("Cube.obj")->renderObj();
	Shader::popMatrixModel();
}
