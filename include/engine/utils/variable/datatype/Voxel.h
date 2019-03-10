#pragma once

#include "Macros.h"

// X+
#define FACE_NORTH		(2 << 0)
// X-
#define FACE_SOUTH		(2 << 1)
// Z+
#define FACE_EAST		(2 << 2)
// Z-
#define FACE_WEST		(2 << 3)
// Y+
#define FACE_TOP		(2 << 4)
// Y-
#define FACE_BOTTOM		(2 << 5)
// All
#define FACE_ALL		((2 << 6) - 1)

struct Voxel {
	Sint8 interactionType = 0;
	Uint16 color = 0;

	Voxel() {};
	Voxel(Sint8 p_interaction, Uint16 p_color) : interactionType(p_interaction), color(p_color) {};

	bool operator==(Voxel p_vox) {
		return (interactionType == p_vox.interactionType &&
				color == p_vox.color);
	}
};
