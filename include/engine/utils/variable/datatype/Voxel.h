#pragma once

#include "Macros.h"

#define FACE_NORTH		(2 << 0) // X+
#define FACE_SOUTH		(2 << 1) // X-
#define FACE_EAST		(2 << 2) // Z+
#define FACE_WEST		(2 << 3) // Z-
#define FACE_TOP		(2 << 4) // Y+
#define FACE_BOTTOM		(2 << 5) // Y-
#define FACE_ALL		((2 << 6) - 1)

struct Voxel
{
	Sint8 interactionType;
	Uint16 color;

	Voxel() {};
	Voxel(Sint8 p_interaction, Uint16 p_color) : interactionType(p_interaction), color(p_color) {};

	bool operator==(Voxel p_vox)
	{
		return (interactionType == p_vox.interactionType &&
				color == p_vox.color);
	}
};
