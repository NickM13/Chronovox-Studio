#pragma once

#include "Macros.h"
#include "Color.h"

// X+
constexpr char FACE_NORTH = (2 << 0);
// X-
constexpr char FACE_SOUTH = (2 << 1);
// Z+
constexpr char FACE_EAST = (2 << 2);
// Z-
constexpr char FACE_WEST = (2 << 3);
// Y+
constexpr char FACE_TOP = (2 << 4);
// Y-
constexpr char FACE_BOTTOM = (2 << 5);
// All
constexpr char FACE_ALL = ((2 << 6) - 1);

struct Voxel {
	Sint8 interactionType = 0;
	Color color;

	Voxel(Sint8 p_interaction = 0, Color p_color = Color()) : interactionType(p_interaction), color(p_color) {};

	bool operator<(const Voxel& vox) const {
		return (interactionType < vox.interactionType ||
			(interactionType == vox.interactionType && color < vox.color));
	}
	bool operator==(Voxel p_vox) const {
		return (interactionType == p_vox.interactionType &&
				color == p_vox.color);
	}
};
