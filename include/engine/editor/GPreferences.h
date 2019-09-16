#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\logger\Logger.h"

class GPreferences {
public:
	enum class ViewMode : Sint8 {
		PERSPECTIVE,
		ORTHOGONAL,
		ISOMETRIC
	};
	enum class AntiAlias : Sint8 {
		NONE,
		LOW,
		MEDIUM,
		HIGH
	};
private:
	static ViewMode m_viewMode;
	static bool m_vsync;
	static Sint32 m_focusFps, m_unfocusFps;
	static Logger::Verbosity m_logVerbosity;
	static AntiAlias m_antiAlias;
	static bool m_voxelAO;
	static Sint32 m_gridCount;
	static Sint32 m_gridSpace;
public:
	static void init();
	static void save();

	static ViewMode getViewMode()			{ return m_viewMode; }
	static 	void setViewMode(ViewMode mode)	{ m_viewMode = mode; }

	static 	bool hasVSync()					{ return m_vsync; }
	static 	void setVSync(bool sync)		{ m_vsync = sync; }

	static Sint32 getFocusFPS()				{ return m_focusFps; }
	static Sint32 getUnfocusFPS()			{ return m_unfocusFps; }
	static void setFocusFPS(Sint32 fps)		{ m_focusFps = fps; }
	static void setUnfocusFPS(Sint32 fps)	{ m_unfocusFps = fps; }

	static Logger::Verbosity getLogVerbosity()			{ return m_logVerbosity; }
	static void setLogVerbosity(Logger::Verbosity verb)	{ m_logVerbosity = verb; }

	static AntiAlias getAntiAlias()						{ return m_antiAlias; }
	static void setAntiAlias(AntiAlias alias)			{ m_antiAlias = alias; }
	static float getSamples();

	static bool hasVoxelAO()				{ return m_voxelAO; }
	static void setVoxelAO(bool ao)			{ m_voxelAO = ao; }

	static 	Sint32 getGridCount()			{ return m_gridCount; }
	static void setGridCount(Sint32 size)	{ m_gridCount = size; }

	static 	Sint32 getGridSpace()			{ return m_gridSpace; }
	static void setGridSpace(Sint32 size)	{ m_gridSpace = size; }
};
