#pragma once
#include "..\datatype\Voxel.h"
#include <set>
#include <vector>
#include <map>

class DVoxel {
private:
	static std::vector<Voxel> m_voxelList;
	static std::map<Voxel, size_t> m_voxelMap;
public:
	static void init();
	static void terminate();
	static size_t addVoxel(Voxel& vox);
	static Voxel& getVoxel(size_t index);
	static size_t getVoxelId(Voxel& voxel);
	static std::vector<Voxel>& getVoxelList();
};
