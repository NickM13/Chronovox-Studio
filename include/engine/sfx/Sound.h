#pragma once

#include "LOpenAL.h"
#include "engine\utils\Singleton.h"
#include "engine\utils\Utilities.h"

//Stores sound
class Buffer
{
public:
	Buffer(std::string p_fileName = "");
	std::string getName() { return m_name; }
	ALuint getId() { return m_bufferId; }
private:
	std::string m_name;
	ALuint m_bufferId;
};

class MBuffer : public Singleton<MBuffer>
{
protected:
	std::vector<Buffer> m_unitList;
public:
	MBuffer() {};
	MBuffer(const MBuffer&) {};
	MBuffer operator=(const MBuffer&) {};
	~MBuffer() {
		m_unitList.clear();
	}

	// Create unit, no matter if it exists or not
	Uint32 addUnit(Buffer& p_unit)
	{
		m_unitList.push_back(p_unit);
		return Uint32(m_unitList.size() - 1);
	}
	// Create unit, no matter if it exists or not
	Uint32 addUnit(std::string p_name)
	{
		m_unitList.push_back(Buffer(p_name));
		return Uint32(m_unitList.size() - 1);
	}

	bool contains(std::string p_name)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_name == m_unitList[i].getName())
				return true;
		return false;
	}

	Buffer& getUnit(Uint32 p_guid)
	{
		return m_unitList.at(p_guid);
	}
	Buffer& getUnit(std::string p_name)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_name == m_unitList[i].getName())
				return m_unitList[i];
		return getUnit(addUnit(p_name));
	}

	std::vector<Buffer>& getUnitList()
	{
		return m_unitList;
	}

	// Look for unit, if it doesnt exist it adds it
	Uint32 getUnitID(std::string p_name)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_name == m_unitList[i].getName())
				return i;
		return addUnit(p_name);
	}

	// Look for unit, if it doesnt exist it adds it
	void removeUnit(Uint16 p_guid)
	{
		m_unitList.erase(m_unitList.begin() + p_guid);
	}

};

//Stores buffer id, plays sound
class Source
{
public:
	Source(std::string p_sourceName = "");
	void setSound(Buffer);

	void setPosition(Vector3<GLfloat>);
	void setVelocity(Vector3<GLfloat>);

	void play();
	void play(Vector3<GLfloat> pos);
	void play2d();
	std::string getName() { return m_name; }
private:
	std::string m_name;
	ALuint m_sourceId;
	Buffer m_buffer;
	Vector3<GLfloat> m_pos;
	Vector3<GLfloat> m_vel;
};

class Sound : public Singleton<Sound>
{
public:
	bool init();
	void terminate();

	void setListenPosition(Vector3<GLfloat>);
	void setListenVelocity(Vector3<GLfloat>);
	void setListenOrientation(Vector3<GLfloat> p_lookAt);
private:
	ALCdevice* m_device;
	ALCcontext* m_context;
};
