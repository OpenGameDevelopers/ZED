#ifndef __ZED_SYSTEM_INPUTTYPES_HPP__
#define __ZED_SYSTEM_INPUTTYPES_HPP__

#include <DataTypes.hpp>

// Gamepad controls
enum ZED_GAMEPAD
{
	ZED_GAMEPAD_LTRIGGER,
	ZED_GAMEPAD_RTRIGGER,

	ZED_GAMEPAD_LSTICKX,
	ZED_GAMEPAD_LSTICKY,

	ZED_GAMEPAD_RSTICKX,
	ZED_GAMEPAD_RSTICKY,
	
	// Dreamcast:	[Disabled], [Disabled]
	ZED_GAMEPAD_LSTICK_BUTTON,
	ZED_GAMEPAD_RSTICK_BUTTON,
	
	// Dreamcast:	Start, [Disabled]
	ZED_GAMEPAD_START,
	ZED_GAMEPAD_BACK,
	
	// Xbox (360):	D-Pad
	// Dreamcast:	D-Pad
	ZED_GAMEPAD_LPAD_UP,
	ZED_GAMEPAD_LPAD_DOWN,
	ZED_GAMEPAD_LPAD_LEFT,
	ZED_GAMEPAD_LPAD_RIGHT,

	// Xbox (360):	Y, A, X, B
	// Dreamcast:	Y, A, X, B
	ZED_GAMEPAD_LPAD_UP,
	ZED_GAMEPAD_LPAD_DOWN,
	ZED_GAMEPAD_LPAD_LEFT,
	ZED_GAMEPAD_LPAD_RIGHT.

	// Xbox:		White, Black
	// Xbox 360:	L-Bumper, R-Bumper
	// Dreamcast:	Disabled
	ZED_GAMEPAD_LSHOULDER,
	ZED_GAMEPAD_RSHOULDER,
};

typedef struct __INPUT_LIST
{
	// Only one mouse and keyboard are supported
	ZED_BOOL	Keyboard;
	ZED_BOOL	Mouse;
	// Gamepads can be lightguns, controllers, wheels, and fishing rods
	ZED_BYTE	Gamepads;
	ZED_BYTE	MemoryUnits;
}INPUT_LIST, *LPINPUT_LIST;

#endif

