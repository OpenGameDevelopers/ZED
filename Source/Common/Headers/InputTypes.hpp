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
	ZED_GAMEPAD_RPAD_UP,
	ZED_GAMEPAD_RPAD_DOWN,
	ZED_GAMEPAD_RPAD_LEFT,
	ZED_GAMEPAD_RPAD_RIGHT,

	// Xbox:		White, Black
	// Xbox 360:	L-Bumper, R-Bumper
	// Dreamcast:	Disabled
	ZED_GAMEPAD_LSHOULDER,
	ZED_GAMEPAD_RSHOULDER,
};

typedef enum __ZED_KEY
{
	K_TAB					= 9,
	K_ENTER					= 13,
	K_ESCAPE				= 27,
	K_SPACE					= 32,

	K_BACKSPACE				= 127,

	K_COMMAND				= 128,
	K_CAPSLOCK,
	K_SCROLL,
	K_POWER,
	K_PAUSE,

	K_UPARROW				= 133,
	K_DOWNARROW,
	K_LEFTARROW,
	K_RIGHTARROW,

	K_LSUPER				= 137,
	K_RSUPER,
	K_SUPER,

	K_ALT					= 140,
	K_CTRL,
	K_SHIFT,
	K_INS,
	K_DEL,
	K_PGDN,
	K_PGUP,
	K_HOME,
	K_END,

	K_F1					= 149,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_INVERSE_EXCLAMATION	= 161,
	K_F13,
	K_F14,
	K_F15,

	K_NP_HOME				= 165,
	K_NP_UPARROW,
	K_NP_PGUP,
	K_NP_LEFTARROW,
	K_NP_5,
	K_NP_RIGHTARROW,
	K_NP_END,
	K_NP_DOWNARROW,
	K_NP_PGDN,
	K_NP_ENTER,
	K_NP_INS,
	K_NP_DEL,
	K_NP_SLASH,
	K_SUPERSCRIPT_TWO		= 178,
	K_NP_MINUS,
	K_ACUTE_ACCENT			= 180,
	K_NP_PLUS,
	K_NP_NUMLOCK,
	K_NP_ASTERISK,
	K_NP_EQUALS,
}ZED_KEY;

typedef struct __ZED_KEYSTROKE
{
	ZED_BYTE	Key;
	ZED_BYTE	Flags;
	ZED_CHAR8	ASCII;
	ZED_BYTE	__Padding;
}ZED_KEYSTROKE;

typedef struct __INPUT_LIST
{
	// Only one mouse and keyboard are supported
	ZED_BOOL	Keyboard;
	ZED_BOOL	Mouse;
	// Gamepads can be lightguns, controllers, wheels, and fishing rods
	ZED_BYTE	Gamepads;
	ZED_BYTE	MemoryUnits;
}INPUT_LIST, *LPINPUT_LIST;

const ZED_UINT32 ZED_INPUT_DEVICE_KEYBOARD	= 0x00000001;
const ZED_UINT32 ZED_INPUT_DEVICE_MOUSE		= 0x00000002;
const ZED_UINT32 ZED_INPUT_DEVICE_GAMEPAD	= 0x00000004;

#endif

