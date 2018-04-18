#pragma once
#include "engine\utils\Utilities.h"
#include "engine\gfx\texture\MTexture.h"

class Camera {
private:
	static Vector3<GLfloat> m_position, m_rotation;
	static GLfloat m_zoom, m_tarZoom, m_zoomSpeed;
	static Texture* m_skyTexture;

	static void zoom(GLfloat p_scroll);
	static void turn(Vector2<Sint32> p_mouseMove);
	static void pan(Vector2<Sint32> p_mouseMove);

public:
	static void init();
	static void reset();

	static void setPosition(Vector3<GLfloat> p_position);

	static Vector3<GLfloat> getFocus();
	static Vector3<GLfloat> getPosition();
	static Vector3<GLfloat> getDirection();
	static Vector3<GLfloat> getMouseDirection(); // TODO: For some reason this only works with fov=70

	static void applyTransformation();

	static void input(Sint8 p_guiFlags);
	static void update(GLfloat p_deltaUpdate);
	static void renderSkybox();
	static void renderFocus();
};
