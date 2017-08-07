#include "engine\editor\model\Model.h"
#include "engine\utils\Globals.h"
#include "engine\utils\Utilities.h"
#include "engine\utils\variable\manager\ColorManager.h"
#include "engine\gfx\gui\Component.h"
#include "engine\editor\menu\Overlay.h"

#include <direct.h>
#include <shlobj.h>

#include <iostream>

Model::Model()
{
	m_matrixEdit = new EditMatrix();
	m_skyTexture = LTexture::getTexture("Daylight Sky.png");
	
	m_camPos = {0, 6, 0};

	m_zoom = 32;
	m_tarZoom = 0;
	m_zoomSpeed = 12.f;

	m_selectedMatrix = -1;
	m_dragging = NONE;
	m_matrixCopy = 0;

	addMatrix("Box", {-5, 0, -5}, {10, 10, 10});

	m_grid = true;
	m_outline = true;
}
Model::~Model()
{
	while(m_matrices.size() > 0)
	{
		delete m_matrices[0];
		m_matrices.erase(m_matrices.begin());
	}
	delete m_matrixEdit;
	delete r, g, b;
}

void Model::setColor(Sint32& p_r, Sint32& p_g, Sint32& p_b)
{
	r = &p_r;
	g = &p_g;
	b = &p_b;

	*r = 0;
	*g = 255;
	*b = 0;
}

void Model::toggleGrid()
{
	m_grid = !m_grid;
}
void Model::toggleOutline()
{
	m_outline = !m_outline;
}

void Model::zoom(GLfloat p_scroll)
{
	m_tarZoom += p_scroll;
	if(m_zoom - m_tarZoom < 1)
		m_tarZoom = m_zoom - 1;
	else if(m_zoom - m_tarZoom > 256)
		m_tarZoom = m_zoom - 256;
}
void Model::turn(Vector2<Sint32> p_mouseMove)
{
	m_camRot = m_camRot + Vector3<GLfloat>(GLfloat(p_mouseMove.y), GLfloat(p_mouseMove.x), 0) * 0.5f;

	if(m_camRot.x < -90) m_camRot.x = -90;
	if(m_camRot.x > 90) m_camRot.x = 90;

	m_camRot.y = fmodf(m_camRot.y, 360);
}
void Model::pan(Vector2<Sint32> p_mouseMove)
{
	Vector3<GLfloat> _norm = Math::computeDirection(m_camRot + Vector3<GLfloat>(90, 0, 0)) * Vector3<GLfloat>{1, -1, 1};

	GLfloat _sensitivity = m_zoom * 0.0015f + 0.0035f;
	m_camPos.x -= p_mouseMove.x * _sensitivity * Math::cosd(m_camRot.y);
	m_camPos.z -= p_mouseMove.x * _sensitivity * Math::sind(m_camRot.y);
	m_camPos = m_camPos + _norm * p_mouseMove.y * _sensitivity;
}

void Model::focus()
{
	m_camPos = m_matrixEdit->getPos() + Vector3<GLfloat>(m_matrixEdit->getSize()) / 2.f;
}

void Model::undo()
{
	m_matrixEdit->undo();
}
void Model::redo()
{
	m_matrixEdit->redo();
}
void Model::removeMatrix()
{
	if(*m_tool > SELECT && m_matrixEdit->getId() != -1)
	{
		DeleteCommand* _cmd = new DeleteCommand(m_matrices[m_matrixEdit->getId()], m_matrices, m_matrixEdit->getId());
		m_matrixEdit->addCommand(_cmd);

		Sint32 id = m_matrixEdit->getId();
		m_matrixEdit->unloadMatrix();
		m_matrices.erase(m_matrices.begin() + id);

		for(Uint16 i = id; i < m_matrices.size(); i++)
			m_matrices[i]->setId(m_matrices[i]->getId() - 1);
	}
}

void Model::copyMatrix()
{
	if(m_matrixCopy) delete m_matrixCopy;
	if(m_matrixEdit->getId() != -1)
		m_matrixCopy = new Matrix(*m_matrices[m_matrixEdit->getId()]);
}
void Model::pasteMatrix()
{
	if(m_matrixCopy)
	{
		m_matrixCopy->setPosition(m_camPos - (m_matrixCopy->getSize() / 2));
		m_matrices.push_back(new Matrix(*m_matrixCopy));
		NewCommand* _cmd = new NewCommand(m_matrixCopy, m_matrices, Uint16(m_matrices.size()) - 1);
		m_matrixEdit->addCommand(_cmd);
	}
}

void Model::setVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos, Voxel p_voxel)
{
	m_matrices[p_matrix]->setVoxel(p_pos, p_voxel);
}
Voxel Model::getVoxel(Uint16 p_matrix, Vector3<Sint32> p_pos)
{
	return m_matrices[p_matrix]->getVoxel(p_pos);
}
Uint16 Model::getVoxelId(Uint16 p_matrix, Vector3<Sint32> p_pos)
{
	return m_matrices[p_matrix]->getVoxelId(p_pos);
}

void Model::resize(Uint16 p_matrixId, Vector3<Sint32> p_offset, Vector3<Sint32> p_size)
{
	if(p_size.x <= p_offset.x || p_size.x <= p_offset.x || p_size.x <= p_offset.x) return;

	Sint32 id = m_matrixEdit->getId();
	m_matrixEdit->unloadMatrix();

	Matrix* prev = new Matrix(*m_matrices[p_matrixId]);
	m_matrices[p_matrixId]->setSize(p_size);

	m_matrices[p_matrixId]->shiftVoxels(p_offset * -1);
	m_matrices[p_matrixId]->setSize(p_size - p_offset);
	m_matrices[p_matrixId]->addPosition(p_offset);

	ResizeCommand* _cmd = new ResizeCommand(m_matrices[p_matrixId], prev);
	m_matrixEdit->addCommand(_cmd);

	if(id != -1)
		m_matrixEdit->setMatrix(m_matrices[id], id);
}
void Model::shiftMatrix(Vector3<Sint32> p_direction)
{
	if(m_selectedMatrix != -1)
	{
		m_matrices[m_selectedMatrix]->shiftVoxels(p_direction);
	}
}
void Model::flipMatrix(Sint8 p_axesFlags)
{
	if(m_matrixEdit->getId() != -1)
	{
		m_matrices[m_matrixEdit->getId()]->flip(p_axesFlags);
		m_matrixEdit->setMatrix(m_matrices[m_matrixEdit->getId()], m_matrixEdit->getId());
	}
}
void Model::addMatrix(std::string p_name, Vector3<GLfloat> p_pos, Vector3<Sint16> p_size)
{
	Sint16 id = Sint16(m_matrices.size());
	m_matrices.push_back(new Matrix(p_name, id, (Vector3<GLfloat>(p_size.x, 0, p_size.z) / -2), p_size));
	NewCommand* _cmd = new NewCommand(m_matrices[m_matrices.size() - 1], m_matrices, Uint16(m_matrices.size()) - 1);
	m_matrixEdit->addCommand(_cmd);
	if(m_tool) *m_tool = 1;
	m_selectedMatrix = Sint16(m_matrices.size()) - 1;
	m_matrixEdit->setMatrix(m_matrices[m_selectedMatrix], m_selectedMatrix);
}
void Model::renameMatrix(Uint16 id, std::string p_name)
{
	m_matrices[id]->setName(p_name);
}

Vector3<GLfloat> Model::getCamPosition()
{
	Vector3<GLfloat> _zoomOffset = Math::computeDirection(m_camRot);

	return (m_camPos + _zoomOffset * -m_zoom);
}
Vector3<GLfloat> Model::getCamDirection()
{
	return Math::computeDirection(m_camRot);
}
Vector3<GLfloat> Model::getCamMouseDirection()
{
	Vector2<GLfloat> _mousePos = MouseStates::m_mousePos;
	GLfloat _screenRatio = GLfloat(Globals::m_screenSize.x) / Globals::m_screenSize.y;
	GLfloat _fovx = Globals::m_fov * _screenRatio;
	GLfloat _fovy = Globals::m_fov;
	Vector2<GLfloat> _mousePercent = Vector2<GLfloat>(_mousePos) / Globals::m_screenSize * 2 - 1; // Ranges from -1 to 1
	Vector3<GLfloat> camDir = {
		(_mousePercent.x * _fovx * -Math::cosd(m_camRot.y)) + ((100 * Math::cosd(m_camRot.x) + (_mousePercent.y * _fovy * -Math::sind(m_camRot.x))) * -Math::sind(m_camRot.y)),
		(_mousePercent.y * _fovy * Math::cosd(m_camRot.x)) + (100 * Math::sind(m_camRot.x)),
		(_mousePercent.x * _fovx * -Math::sind(m_camRot.y)) + ((100 * -Math::cosd(m_camRot.x) + (_mousePercent.y * _fovy * Math::sind(m_camRot.x))) * -Math::cosd(m_camRot.y))
	};
	camDir = camDir.getNormal() * -1;
	return camDir;
}

void Model::input(Sint8 p_guiFlags)
{
	Sint8 _mbutton[64];
	if(!(p_guiFlags & Component::EVENT_MOUSEOVER)) for(Uint16 i = 0; i < 64; i++) _mbutton[i] = 0;
	else for(Uint16 i = 0; i < 64; i++) _mbutton[i] = MouseStates::m_mouseStates[i];

	GLfloat _near = (p_guiFlags & Component::EVENT_MOUSEOVER) ? 0 : 1, _far = 1;

	MatrixCast::castRayMatrix(getCamPosition(), getCamMouseDirection() * 4096, m_matrices, m_selectedMatrix, _near, _far);
	if(*m_tool <= SELECT && ((!(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) && m_selectedMatrix != m_matrixEdit->getId()) || _mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_RELEASE))
	{
		if(m_selectedMatrix == -1)	m_matrixEdit->unloadMatrix();
		else						m_matrixEdit->setMatrix(m_matrices[m_selectedMatrix], m_selectedMatrix);
	}

	if(m_matrixEdit->getId() != -1)
	{
		if(*m_tool <= SELECT)
			MatrixCast::castRayVoxel(getCamPosition(), getCamMouseDirection() * 4096, m_matrixEdit->getInitMatrix(), _near, _far, m_selectedVoxel, m_selectedSide, m_selectedVoxelOffset);
		else
			MatrixCast::castRayScale(getCamPosition(), getCamMouseDirection() * 4096, m_matrixEdit->getMatrix(), _near, _far, m_scalePos, m_selectedScale);
	}

	if((_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_RELEASE) || !(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN))
	{
		MatrixMove::end();
		Box::end();
		MatrixResize::end();
	}

	MatrixMove::update(getCamPosition(), getCamMouseDirection());
	switch(*m_tool)
	{
	case ADD: Box::update(m_selectedVoxelOffset); break;
	case ERASE: case REPLACE: Box::update(m_selectedVoxel); break;
	default: break;
	}
	MatrixResize::update(getCamPosition(), getCamDirection(), getCamMouseDirection());

	if((_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) && *m_tool > SELECT || m_selectedMatrix != -1)
	{
		Color color;
		switch(*m_tool)
		{
		case ADD:
			switch(*m_toolMeta)
			{
			case 0: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) m_matrices[m_selectedMatrix]->setVoxel(m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f)))); break;
			case 1: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Box::start(m_matrices[m_selectedMatrix], m_selectedVoxelOffset, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f))), false); break;
			case 2: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Fill::append(m_matrices[m_selectedMatrix], m_selectedVoxelOffset, m_selectedSide, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f)))); break;
			default: break;
			}
			break;
		case ERASE:
			switch(*m_toolMeta)
			{
			case 0: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) m_matrices[m_selectedMatrix]->setVoxel(m_selectedVoxel, Voxel(0, MColor::getInstance().getUnitID(Color()))); break;
			case 1: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Box::start(m_matrices[m_selectedMatrix], m_selectedVoxel, Voxel(0, MColor::getInstance().getUnitID(Color())), false); break;
			case 2: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Fill::insert(m_matrices[m_selectedMatrix], m_selectedVoxel, m_selectedSide, Voxel(0, MColor::getInstance().getUnitID(Color()))); break;
			default: break;
			}
			break;
		case REPLACE:
			switch(*m_toolMeta)
			{
			case 0: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN) m_matrices[m_selectedMatrix]->setVoxel(m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f)))); break;
			case 1: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Box::start(m_matrices[m_selectedMatrix], m_selectedVoxel, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f))), true); break;
			case 2: if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS) Fill::insert(m_matrices[m_selectedMatrix], m_selectedVoxel, m_selectedSide, Voxel(1, MColor::getInstance().getUnitID(Color(*r / 255.f, *g / 255.f, *b / 255.f)))); break;
			default: break;
			}
			break;
		case EYEDROP:
			if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_DOWN)
			{
				color = MColor::getInstance().getUnit(m_matrices[m_selectedMatrix]->getVoxel(m_selectedVoxel).color);
				*r = color.r * 255;
				*g = color.g * 255;
				*b = color.b * 255;
			}
			break;
		case SELECT:
			if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			{
				if(m_selectedMatrix != -1)
					m_matrixEdit->setMatrix(m_matrices[m_selectedMatrix], m_selectedMatrix);
				else
					m_matrixEdit->unloadMatrix();
			}
			break;
		case MOVE:
			if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			{
				if(m_matrixEdit->getId() != -1 && m_selectedScale != 0)
					MatrixMove::start(m_matrixEdit, m_scalePos, m_selectedScale);
				else
				{
					if(m_selectedMatrix != -1)
						m_matrixEdit->setMatrix(m_matrices[m_selectedMatrix], m_selectedMatrix);
					else
						m_matrixEdit->unloadMatrix();
				}
			}
			break;
		case RESIZE:
			if(_mbutton[GLFW_MOUSE_BUTTON_LEFT] & MouseStates::MOUSE_PRESS)
			{
				if(m_matrixEdit->getId() != -1 && m_selectedScale != 0)
					MatrixResize::start(m_matrixEdit, m_scalePos, m_selectedScale);
				else
				{
					if(m_selectedMatrix != -1)
						m_matrixEdit->setMatrix(m_matrices[m_selectedMatrix], m_selectedMatrix);
					else
						m_matrixEdit->unloadMatrix();
				}
			}
			break;
		default:

			break;
		}
	}
}
void Model::update(GLfloat p_deltaUpdate)
{
	GLfloat _zoom = m_tarZoom * m_zoomSpeed * p_deltaUpdate;

	m_zoom -= _zoom;
	m_tarZoom -= _zoom;

	if(m_matrixEdit->getId() != -1)
	{
		Matrix* m = m_matrices[m_matrixEdit->getId()];
		*m_dataString = std::string("(" + Util::numToString(m_matrixEdit->getMatrix()->getId()) + ") \"" + m->getName() + "\" : {" + Util::numToString(m->getPos().x, 1) + ", " + Util::numToString(m->getPos().y, 1) + ", " + Util::numToString(m->getPos().z, 1) + "} : {" + Util::numToString(m->getSize().x) + ", " + Util::numToString(m->getSize().y) + ", " + Util::numToString(m->getSize().z) + "}");
	}
	else
		*m_dataString = "";
}
void Model::render()
{
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		glTranslatef(0, 0, -m_zoom);
		glRotatef(m_camRot.x, 1, 0, 0);
		glRotatef(m_camRot.y, 0, 1, 0);
		glRotatef(m_camRot.z, 0, 0, 1);
		renderFocus();
		glTranslatef(-m_camPos.x, -m_camPos.y, -m_camPos.z);
		if(m_grid) renderGrid();

		glDepthFunc(GL_LEQUAL);
		for(Uint16 i = 0; i < m_matrices.size(); i++)
		{
			if(m_outline)		m_matrices[i]->render((m_matrixEdit->getId() == i ? HIGHLIGHT : OUTLINE), 0);
			else					m_matrices[i]->render(OutlineType::NONE, 0);
		}

		renderSelected();
	}
	glPopMatrix();
}
void Model::renderSelected()
{
	if(*m_tool <= SELECT && m_matrixEdit->getId() != -1)
	{
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			Vector3<GLfloat> _mPos = m_matrixEdit->getPos();
			Vector3<GLfloat> _offset = _mPos + m_selectedVoxel + 0.5f;
			GLfloat _scale = 0.5025f;

			if(Box::running())
			{
				glTranslatef(_mPos.x, _mPos.y, _mPos.z);
				Box::render();
			}
			else
			{
				glTranslatef(_offset.x, _offset.y, _offset.z);
				glScalef(_scale, _scale, _scale);
				glColor4f(1, 1, 1, 0.5f);
				glBegin(GL_QUADS);
				{
					switch(m_selectedSide)
					{
					case FACE_NORTH:
						glVertex3f(1, 1, -1);
						glVertex3f(1, 1, 1);
						glVertex3f(1, -1, 1);
						glVertex3f(1, -1, -1);
						break;
					case FACE_SOUTH:
						glVertex3f(-1, 1, -1);
						glVertex3f(-1, -1, -1);
						glVertex3f(-1, -1, 1);
						glVertex3f(-1, 1, 1);
						break;
					case FACE_EAST:
						glVertex3f(-1, 1, 1);
						glVertex3f(-1, -1, 1);
						glVertex3f(1, -1, 1);
						glVertex3f(1, 1, 1);
						break;
					case FACE_WEST:
						glVertex3f(-1, 1, -1);
						glVertex3f(1, 1, -1);
						glVertex3f(1, -1, -1);
						glVertex3f(-1, -1, -1);
						break;
					case FACE_TOP:
						glVertex3f(-1, 1, 1);
						glVertex3f(1, 1, 1);
						glVertex3f(1, 1, -1);
						glVertex3f(-1, 1, -1);
						break;
					case FACE_BOTTOM:
						glVertex3f(1, -1, -1);
						glVertex3f(1, -1, 1);
						glVertex3f(-1, -1, 1);
						glVertex3f(-1, -1, -1);
						break;
					default:
						break;
					}
				}
				glEnd();
			}
		}
		glPopMatrix();
	}
	else if((*m_tool == MOVE || *m_tool == RESIZE) && m_matrixEdit->getId() != -1)
	{
		Vector3<GLfloat> s = Vector3<GLfloat>(m_matrices[m_matrixEdit->getId()]->getSize()) / 2.f;
		Vector3<GLfloat> _offset = m_matrices[m_matrixEdit->getId()]->getPos() + s;
		glPushMatrix();
		{
			glTranslatef(_offset.x, _offset.y, _offset.z);

			SVoxel::render({-s.x, -0.5f, -0.5f}, {-0.25f, 1, 1}, {1, 0, 0, (m_selectedScale == FACE_SOUTH ? 1 : 0.5f)});
			SVoxel::render({s.x, -0.5f, -0.5f}, {0.25f, 1, 1}, {1, 0, 0, (m_selectedScale == FACE_NORTH ? 1 : 0.5f)});

			SVoxel::render({-0.5f, -s.y, -0.5f}, {1, -0.25f, 1}, {0, 1, 0, (m_selectedScale == FACE_BOTTOM ? 1 : 0.5f)});
			SVoxel::render({-0.5f, s.y, -0.5f}, {1, 0.25f, 1}, {0, 1, 0, (m_selectedScale == FACE_TOP ? 1 : 0.5f)});

			SVoxel::render({-0.5f, -0.5f, -s.z}, {1, 1, -0.25f}, {0, 0, 1, (m_selectedScale == FACE_WEST ? 1 : 0.5f)});
			SVoxel::render({-0.5f, -0.5f, s.z}, {1, 1, 0.25f}, {0, 0, 1, (m_selectedScale == FACE_EAST ? 1 : 0.5f)});
		}
		glPopMatrix();
	}
}
void Model::renderGrid()
{
	glPushMatrix();
	{
		glTranslatef(0, -0.025f, 0);
		glBegin(GL_LINES);
		{
			for(Sint16 i = -32; i <= 32; i++)
			{
				if(i == 0)
				{
					glColor3f(0.5f, 0.5f, 0.5f);
					glVertex3f(i, 0, -32);
					glVertex3f(i, 0, 0);
					glVertex3f(0, 0, i);
					glVertex3f(-32, 0, i);

					glColor3f(1, 0, 0);
					glVertex3f(0, 0, i);
					glVertex3f(32, 0, i);

					glColor3f(0, 0, 1);
					glVertex3f(i, 0, 0);
					glVertex3f(i, 0, 32);
				}
				else
				{
					glColor3f(0.5f, 0.5f, 0.5f);
					glVertex3f(-32, 0, i);
					glVertex3f(32, 0, i);
					glVertex3f(i, 0, -32);
					glVertex3f(i, 0, 32);
				}
			}
			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 16, 0);
		}
		glEnd();
	}
	glPopMatrix();
}
void Model::renderFocus()
{
	glPushMatrix();
	{
		GLfloat _scale = m_zoom / 256.f;
		glScalef(_scale, _scale, _scale);
		SVoxel::render({-1, -1, -1}, {2, 2, 2}, {1, 0, 0, 1});
	}
	glPopMatrix();
}
void Model::renderSkyBox()
{
	glPushMatrix();
	{
		glRotatef(m_camRot.x, 1, 0, 0);
		glRotatef(m_camRot.y, 0, 1, 0);
		glRotatef(m_camRot.z, 0, 0, 1);

		glBindTexture(GL_TEXTURE_2D, m_skyTexture.getId());
		glColor3f(1, 1, 1);
		glScalef(1024, 1024, 1024);

		Vector2<GLfloat> _c = Vector2<GLfloat>(4.f, 4.f) / m_skyTexture.getSize();
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
	}
	glPopMatrix();
}

std::vector<std::string> Model::getMatrixNames()
{
	std::vector<std::string> names;

	for(Uint16 i = 0; i < m_matrices.size(); i++)
		names.push_back("(" + Util::numToString(i) + ") " + m_matrices[i]->getName());

	return names;
}
Matrix* Model::getMatrix(Sint16 id)
{
	if(id == -1) return 0;
	return m_matrices[id];
}

void Model::save()
{
	char documents[MAX_PATH];
	HRESULT r = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);

	strcat_s(documents, "\\VoxelModels");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Nick's Voxel Model\0*.nvm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if(r == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save As";
	if(!GetSaveFileName(&ofn)) return;

	save(filename);
}
void Model::open()
{
	char documents[MAX_PATH];
	HRESULT r = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, documents);
	
	strcat_s(documents, "\\VoxelModels");
	_mkdir(documents);

	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Nick's Voxel Model\0*.nvm*\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	if(r == S_OK) ofn.lpstrInitialDir = documents;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Open Model";
	if(!GetOpenFileName(&ofn)) return;

	load(filename);
}
void Model::newModel()
{
	
}

void Model::save(std::string p_fileName)
{
	NvmFormat::save(p_fileName, m_matrices);
}

void Model::load(std::string p_fileName)
{
	if(NvmFormat::load(p_fileName, m_matrices))
		m_matrixEdit->unloadMatrix(false);
}
