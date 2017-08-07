#pragma once
#include "..\..\Singleton.h"
#include "..\..\Utilities.h"
#include <vector>

template<class T>
class Manager : public Singleton<Manager<T>>
{
protected:
	std::vector<T> m_unitList;
public:
	Manager() {};
	Manager(const Manager&) {};
	Manager operator=(const Manager&) {};

	// Create unit, no matter if it exists or not
	Uint32 addUnit(T& p_unit)
	{
		m_unitList.push_back(p_unit);
		return Uint32(m_unitList.size() - 1);
	}

	bool contains(T& p_unit)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_unit == m_unitList[i])
				return true;
		return false;
	}

	T& getUnit(Uint32 p_guid)
	{
		return m_unitList.at(p_guid);
	}

	std::vector<T>& getUnitList()
	{
		return m_unitList;
	}

	// Look for unit, if it doesnt exist it adds it
	Uint32 getUnitID(T& p_unit)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_unit == m_unitList[i])
				return i;
		return addUnit(p_unit);
	}

	// Look for unit, if it doesnt exist it adds it
	void removeUnit(Uint16 p_guid)
	{
		m_unitList.erase(m_unitList.begin() + p_guid);
	}
};



template<class T>
class ManagerEntity : public Singleton<ManagerEntity<T>>
{
protected:
	std::vector<T> m_unitList;
public:
	ManagerEntity() {};
	ManagerEntity(const ManagerEntity&) {};
	ManagerEntity operator=(const ManagerEntity&) {};

	// Create unit, no matter if it exists or not
	Uint32 addUnit(T p_unit)
	{
		m_unitList.push_back(p_unit);
		return Uint32(m_unitList.size() - 1);
	}

	bool contains(T p_unit)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_unit == m_unitList[i])
				return true;
		return false;
	}

	T getUnit(Uint32 p_guid)
	{
		return m_unitList.at(p_guid);
	}

	std::vector<T>& getUnitList()
	{
		return m_unitList;
	}

	// Look for unit, if it doesnt exist it adds it
	Uint32 getUnitID(T p_unit)
	{
		for(Uint32 i = 0; i < m_unitList.size(); i++)
			if(p_unit == m_unitList[i])
				return i;
		return addUnit(p_unit);
	}

	// Look for unit, if it doesnt exist it adds it
	T removeUnit(Uint16 p_guid)
	{
		T _temp = m_unitList[p_guid];
		m_unitList.erase(m_unitList.begin() + p_guid);
		return _temp;
	}
};