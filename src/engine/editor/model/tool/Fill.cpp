#include "engine\editor\model\tool\Fill.h"

void vectorAdd(std::vector<Vector3<Sint32>> &list1, std::vector<Vector3<Sint32>> list2, Vector3<Sint32> element)
{
	for(Sint16 i = Sint16(list1.size()) - 1; i >= 0; i--)
		if(list1[i] == element)
			return;
	for(Sint16 i = Sint16(list2.size() - 1); i >= 0; i--)
		if(list2[i] == element)
			return;

	list1.push_back(element);
}
void Fill::append(Matrix* p_matrix, Vector3<Sint32> p_pos, Sint8 p_side, Voxel p_fill)
{
	Vector3<Sint32> _size = p_matrix->getSize();
	Voxel baseVoxel = p_matrix->getVoxel(p_pos);
	std::vector<Vector3<Sint32>> voxels, check;
	check.push_back(p_pos);

	switch(p_side)
	{
	case FACE_SOUTH:
		while(check.size() > 0)
		{
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].x == _size.x - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, -1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].x == _size.x - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].x == _size.x - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].x == _size.x - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_NORTH:
		while(check.size() > 0)
		{
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].x == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, -1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].x == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].x == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].x == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_BOTTOM:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].y == _size.y - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].y == _size.y - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (check[0].y == _size.y - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (check[0].y == _size.y - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_TOP:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (p_pos.y == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, -1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (p_pos.y == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, -1, 0)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1)) && (p_pos.y == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1)) && (p_pos.y == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_WEST:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].z == _size.z - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].z == _size.z - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].z == _size.z - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].z == _size.z - 1 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_EAST:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0)) && (check[0].z == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0)) && (check[0].z == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0)) && (check[0].z == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0)) && (check[0].z == 0 || p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, -1)).interactionType == 1)) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	}

	for(Uint16 i = 0; i < voxels.size(); i++)
	{
		p_matrix->setVoxel(voxels[i], p_fill);
	}
}
void Fill::insert(Matrix* p_matrix, Vector3<Sint32> p_pos, Sint8 p_side, Voxel p_fill)
{
	Vector3<Sint32> _size = p_matrix->getSize();
	Voxel baseVoxel = p_matrix->getVoxel(p_pos);
	if(baseVoxel.interactionType == 0) return;
	std::vector<Vector3<Sint32>> voxels, check;
	check.push_back(p_pos);

	GLfloat startTime = glfwGetTime();

	while(check.size() > 0)
	{
		if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
		if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
		if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
		if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
		if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
		if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
		voxels.push_back(check[0]);
		check.erase(check.begin());
	}

	//std::cout << "Filled in " << glfwGetTime() - startTime << " seconds.\n";

	/*
	switch(p_side)
	{
	case FACE_SOUTH:
	case FACE_NORTH:
		while(check.size() > 0)
		{
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_BOTTOM:
	case FACE_TOP:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].z > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, -1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, -1));
			if(check[0].z < _size.z - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 0, 1))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 0, 1));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	case FACE_WEST:
	case FACE_EAST:
		while(check.size() > 0)
		{
			if(check[0].x > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(-1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(-1, 0, 0));
			if(check[0].x < _size.x - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(1, 0, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(1, 0, 0));
			if(check[0].y > 0 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, -1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, -1, 0));
			if(check[0].y < _size.y - 1 && baseVoxel == p_matrix->getVoxel(check[0] + Vector3<Sint32>(0, 1, 0))) vectorAdd(check, voxels, check[0] + Vector3<Sint32>(0, 1, 0));
			voxels.push_back(check[0]);
			check.erase(check.begin());
		}
		break;
	}
	*/

	for(Uint16 i = 0; i < voxels.size(); i++)
	{
		p_matrix->setVoxel(voxels[i], p_fill);
	}
}
