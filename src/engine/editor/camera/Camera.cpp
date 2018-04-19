#include "engine\editor\camera\Camera.h"
#include "engine\editor\menu\EditorOverlay.h"

Vector3<GLfloat> Camera::m_position, Camera::m_rotation;
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
	if(m_zoom - m_tarZoom < 1)
		m_tarZoom = m_zoom - 1;
	else if(m_zoom - m_tarZoom > 256)
		m_tarZoom = m_zoom - 256;
}
void Camera::turn(Vector2<Sint32> p_mouseMove) {
	m_rotation = m_rotation + Vector3<GLfloat>(GLfloat(p_mouseMove.y), GLfloat(p_mouseMove.x), 0) * 0.5f;

	if(m_rotation.x < -90) m_rotation.x = -90;
	if(m_rotation.x > 90) m_rotation.x = 90;

	m_rotation.y = fmodf(m_rotation.y, 360);
}
void Camera::pan(Vector2<Sint32> p_mouseMove) {
	Vector3<GLfloat> _norm = Math::computeDirection(m_rotation + Vector3<GLfloat>(90, 0, 0)) * Vector3<GLfloat>(1, -1, 1);
	if(m_rotation.x < 0) _norm = _norm * Vector3<GLfloat>(-1, 1, -1);
	GLfloat _sensitivity = m_zoom * 0.0015f + 0.0135f;
	m_position.x -= p_mouseMove.x * _sensitivity * Math::cosd(m_rotation.y);
	m_position.z -= p_mouseMove.x * _sensitivity * Math::sind(m_rotation.y);
	m_position = m_position + _norm * p_mouseMove.y * _sensitivity;
}

void Camera::setPosition(Vector3<GLfloat> p_position) {
	m_position = p_position;
}

Vector3<GLfloat> Camera::getFocus() {
	return m_position;
}
Vector3<GLfloat> Camera::getPosition() {
	Vector3<GLfloat> _zoomOffset = Math::computeDirection(m_rotation);
	return (m_position + _zoomOffset * -m_zoom);
}
Vector3<GLfloat> Camera::getDirection() {
	return Math::computeDirection(m_rotation);
}
Vector3<GLfloat> Camera::getMouseDirection() {
	Vector2<GLfloat> _mousePos = GMouse::getMousePos();
	GLfloat _screenRatio = GLfloat(GScreen::m_screenSize.x) / GScreen::m_screenSize.y;
	GLfloat _fovx = GScreen::m_fov * _screenRatio;
	GLfloat _fovy = GScreen::m_fov;
	Vector2<GLfloat> _mousePercent = Vector2<GLfloat>(_mousePos) / GScreen::m_screenSize * 2 - 1; // Ranges from -1 to 1
	Vector3<GLfloat> camDir = {
		(_mousePercent.x * _fovx * -Math::cosd(m_rotation.y)) + ((100 * Math::cosd(m_rotation.x) + (_mousePercent.y * _fovy * -Math::sind(m_rotation.x))) * -Math::sind(m_rotation.y)),
		(_mousePercent.y * _fovy * Math::cosd(m_rotation.x)) + (100 * Math::sind(m_rotation.x)),
		(_mousePercent.x * _fovx * -Math::sind(m_rotation.y)) + ((100 * -Math::cosd(m_rotation.x) + (_mousePercent.y * _fovy * Math::sind(m_rotation.x))) * -Math::cosd(m_rotation.y))
	};
	camDir = camDir.getNormal() * -1;
	return camDir;
}

void Camera::applyTransformation() {
	glTranslatef(0, 0, -m_zoom);
	glRotatef(m_rotation.x, 1, 0, 0);
	glRotatef(m_rotation.y, 0, 1, 0);
	glRotatef(m_rotation.z, 0, 0, 1);
	glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

void Camera::input(Sint8 p_guiFlags) {
	if(!EditorOverlay::getContainer()->isPaused() && (p_guiFlags & (Sint8)Component::EventFlag::MOUSEOVER)) {
		if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOD_SHIFT))
			pan(GMouse::getDeltaMousePos());
		else if(GMouse::mouseDown(GLFW_MOUSE_BUTTON_RIGHT))
			turn(GMouse::getDeltaMousePos());
		zoom(GMouse::getMouseScroll());
	}
}

void Camera::update(GLfloat p_deltaUpdate) {
	GLfloat _zoom = m_tarZoom * m_zoomSpeed * p_deltaUpdate;
	m_zoom -= _zoom;
	m_tarZoom -= _zoom;
}

void Camera::renderSkybox() {
	glPushMatrix();
	{
		glRotatef(m_rotation.x, 1, 0, 0);
		glRotatef(m_rotation.y, 0, 1, 0);
		glRotatef(m_rotation.z, 0, 0, 1);

		m_skyTexture->bind();
		glColor3f(1, 1, 1);
		glScalef(1024, 1024, 1024);

		//std::cout << m_skyTexture->getId() << std::endl;

		Vector2<GLfloat> _c = Vector2<GLfloat>(4.f, 4.f) / m_skyTexture->getSize();
		GLfloat _t1 = 1.f / 3;
		GLfloat _t2 = 2.f / 3;
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.25f - _c.x, _t2 - _c.y); glVertex3f(-1, 1, -1);
			glTexCoord2f(0 + _c.x, _t2 - _c.y); glVertex3f(-1, 1, 1);
			glTexCoord2f(0 + _c.x, _t1 + _c.y); glVertex3f(-1, -1, 1);
			glTexCoord2f(0.25f - _c.x, _t1 + _c.y); glVertex3f(-1, -1, -1);

			glTexCoord2f(0.25f + _c.x, _t2 - _c.y); glVertex3f(-1, 1, -1);
			glTexCoord2f(0.25f + _c.x, _t1 + _c.y); glVertex3f(-1, -1, -1);
			glTexCoord2f(0.5f - _c.x, _t1 + _c.y); glVertex3f(1, -1, -1);
			glTexCoord2f(0.5f - _c.x, _t2 - _c.y); glVertex3f(1, 1, -1);

			glTexCoord2f(0.5f + _c.x, _t2 - _c.y); glVertex3f(1, 1, -1);
			glTexCoord2f(0.5f + _c.x, _t1 + _c.y); glVertex3f(1, -1, -1);
			glTexCoord2f(0.75f - _c.x, _t1 + _c.y); glVertex3f(1, -1, 1);
			glTexCoord2f(0.75f - _c.x, _t2 - _c.y); glVertex3f(1, 1, 1);

			glTexCoord2f(1 - _c.x, _t2 - _c.y); glVertex3f(-1, 1, 1);
			glTexCoord2f(0.75f + _c.x, _t2 - _c.y); glVertex3f(1, 1, 1);
			glTexCoord2f(0.75f + _c.x, _t1 + _c.y); glVertex3f(1, -1, 1);
			glTexCoord2f(1 - _c.x, _t1 + _c.y); glVertex3f(-1, -1, 1);

			glTexCoord2f(0.25f + _c.x, 1 - _c.y); glVertex3f(-1, 1, 1);
			glTexCoord2f(0.25f + _c.x, _t2 + _c.y); glVertex3f(-1, 1, -1);
			glTexCoord2f(0.5f - _c.x, _t2 + _c.y); glVertex3f(1, 1, -1);
			glTexCoord2f(0.5f - _c.x, 1 - _c.y); glVertex3f(1, 1, 1);

			glTexCoord2f(0.25f + _c.x, 0 + _c.y); glVertex3f(-1, -1, 1);
			glTexCoord2f(0.5f - _c.x, 0 + _c.y); glVertex3f(1, -1, 1);
			glTexCoord2f(0.5f - _c.x, _t1 - _c.y); glVertex3f(1, -1, -1);
			glTexCoord2f(0.25f + _c.x, _t1 - _c.y); glVertex3f(-1, -1, -1);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}
void Camera::renderFocus() {
	glPushMatrix();
	{
		Vector3<GLfloat> _size = {0.25f, 0.25f, 0.25f};
		MMesh::render("Cube", m_position, _size, {}, {1, 1, 0, 0.5f});
	}
	glPopMatrix();
}
