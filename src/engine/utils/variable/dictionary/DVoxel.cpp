#include "engine\utils\variable\dictionary\DVoxel.h"

std::vector<Voxel> DVoxel::m_voxelList;
std::map<Voxel, size_t> DVoxel::m_voxelMap;

void DVoxel::init() {
	if (!m_voxelList.empty()) terminate();
	addVoxel(Voxel(0, Color()));
}
void DVoxel::terminate() {
	m_voxelList.clear();
	m_voxelMap.clear();
}
size_t DVoxel::addVoxel(Voxel& vox) {
	if (m_voxelMap.find(vox) == m_voxelMap.end()) {
		m_voxelList.push_back(vox);
		m_voxelMap.insert({ vox, m_voxelList.size() - 1 });
		return m_voxelList.size() - 1;
	} else {
		for (size_t i = 0; i < m_voxelList.size(); i++)
			if (m_voxelList.at(i) == vox)
				return i;
	}
	return 0;
}
Voxel& DVoxel::getVoxel(size_t index) {
	return m_voxelList.at(index);
}
size_t DVoxel::getVoxelId(Voxel& voxel) {
	if (m_voxelMap.find(voxel) != m_voxelMap.end())
		return m_voxelMap.at(voxel);
	return addVoxel(voxel);
}
std::vector<Voxel>& DVoxel::getVoxelList() {
	return m_voxelList;
}
