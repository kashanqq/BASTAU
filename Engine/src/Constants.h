#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

///NameSpace for constants used globally
namespace CONSTANTS
{
	const int MAX_EDITOR_TOOLS = 10;
	const int MAX_SPECTATOR_TOOLS = 10;
	const int MAX_WINDOWS = 10;
	const int MAX_USERMODES = 2;
	const int MAX_CAMERA_MODES = 2;
}

///Global Enums
enum DrawingMode
{
	MODE_CONE,
	MODE_BALLON,
};

///Windows for interface
enum Window
{
	WINDOW_TOOLS,
	WINDOW_LOG,
	WINDOW_UNIT_DATA,
	WINDOW_MULTI_UNIT_DATA,
	WINDOW_FILTER,
	WINDOW_SEARCH,
};

///User Modes
enum UserMode
{
	USERMODE_SPECTATOR,
	USERMODE_EDITOR,
};

///Camera view modes
enum CameraMode
{
	CAMERA_ORBIT,
	CAMERA_FREE,
};

///Tools for Spectator mode
enum ToolsSpectator {
	TS_SELECTOR,
};

///Tools for Editor mode
enum ToolEditor
{
	TE_SELECTOR,
	TE_ADD_NODE,
	TE_DELETE_NODE,
	TE_ADD_LINK,
	TE_DELETE_LINK,
};

#endif // !CONSTANTS_H