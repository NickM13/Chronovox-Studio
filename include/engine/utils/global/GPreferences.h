#pragma once
#include "engine\utils\variable\datatype\Macros.h"
#include "engine\utils\logger\Logger.h"

class Preferences {
public:
	enum class ViewMode {
		PERSPECTIVE,
		ORTHOGONAL,
		ISOMETRIC
	};
	enum class AntiAlias {
		NONE,
		ONE,
		TWO,
		THREE
	};
private:
	ViewMode m_viewMode;
	bool m_vsync;
	Sint32 m_focusFps, m_unfocusFps;
	Logger::Verbosity m_logVerbosity;
	bool m_voxelAO;
	Sint32 m_gridSize;
	Sint32 m_gridSpacing;
public:
	ViewMode getViewMode() const		{ return m_viewMode; }
	void setViewMode(ViewMode mode)		{ m_viewMode = mode; }

	bool hasVSync() const				{ return m_vsync; }
	void setVSync(bool vsync)			{ m_vsync = vsync; }

	Sint32 getFocusFPS() const			{ return m_focusFps; }
	Sint32 getUnfocusFPS() const		{ return m_unfocusFps; }
	void setFocusFPS(Sint32 fps)		{ m_focusFps = fps; }
	void setUnfocusFPS(Sint32 fps)		{ m_unfocusFps = fps; }

	Logger::Verbosity getVerbosity() const		{ return m_logVerbosity; }
	void setVerbosity(Logger::Verbosity verb)	{ m_logVerbosity = verb; }

	bool hasVoxelAO() const				{ return m_voxelAO; }
	void setVoxelAO(bool ao)			{ m_voxelAO = ao; }
	
	Sint32 getGridSize() const			{ return m_gridSize; }
	void setGridSize(Sint32 size)		{ m_gridSize = size; }
	
	Sint32 getGridSpacing() const		{ return m_gridSpacing; }
	void setGridSpacing(Sint32 space)	{ m_gridSpacing = space; }
};