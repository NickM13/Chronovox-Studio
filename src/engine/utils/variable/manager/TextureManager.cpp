#pragma warning(push)
#pragma warning(disable:4172)

#include "engine\utils\variable\manager\TextureManager.h"

Texture& MTexture::findUnit(std::string p_unitName)
{
	for(Uint32 i = 0; i < m_unitList.size(); i++)
	{
		if(m_unitList[i].getName() == p_unitName)
			return m_unitList[i];
	}
	return Texture();
}
Uint32 MTexture::findUnitID(std::string p_unitName)
{
	for(Uint32 i = 0; i < m_unitList.size(); i++)
	{
		if(m_unitList[i].getName() == p_unitName)
			return i;
	}
	return 0;
}

#pragma warning(pop)