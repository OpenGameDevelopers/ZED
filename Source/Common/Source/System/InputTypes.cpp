#include <System/InputTypes.hpp>

namespace ZED
{
	namespace System
	{
		const ZED_CHAR8 *KeyToString( const ZED_KEY p_Key )
		{
			switch( p_Key )
			{
				case ZED_KEY_TAB:
				{
					return "TAB";
				}
				case ZED_KEY_ENTER:
				{
					return "ENTER";
				}
				case ZED_KEY_ESCAPE:
				{
					return "ESCAPE";
				}
				case ZED_KEY_SPACE:
				{
					return "SPACE";
				}
				case ZED_KEY_A:
				{
					return "A";
				}
				case ZED_KEY_B:
				{
					return "B";
				}
				case ZED_KEY_C:
				{
					return "C";
				}
				case ZED_KEY_D:
				{
					return "D";
				}
				case ZED_KEY_E:
				{
					return "E";
				}
				case ZED_KEY_F:
				{
					return "F";
				}
				case ZED_KEY_G:
				{
					return "G";
				}
				case ZED_KEY_H:
				{
					return "H";
				}
				case ZED_KEY_I:
				{
					return "I";
				}
				case ZED_KEY_J:
				{
					return "J";
				}
				case ZED_KEY_K:
				{
					return "K";
				}
				case ZED_KEY_L:
				{
					return "L";
				}
				case ZED_KEY_M:
				{
					return "M";
				}
				case ZED_KEY_N:
				{
					return "N";
				}
				case ZED_KEY_O:
				{
					return "O";
				}
				case ZED_KEY_P:
				{
					return "P";
				}
				case ZED_KEY_Q:
				{
					return "Q";
				}
				case ZED_KEY_R:
				{
					return "R";
				}
				case ZED_KEY_S:
				{
					return "S";
				}
				case ZED_KEY_T:
				{
					return "T";
				}
				case ZED_KEY_U:
				{
					return "U";
				}
				case ZED_KEY_V:
				{
					return "V";
				}
				case ZED_KEY_W:
				{
					return "W";
				}
				case ZED_KEY_X:
				{
					return "X";
				}
				case ZED_KEY_Y:
				{
					return "Y";
				}
				case ZED_KEY_Z:
				{
					return "Z";
				}
				case ZED_KEY_MINUS:
				{
					return "MINUX";
				}
				case ZED_KEY_EQUALS:
				{
					return "EQUALS";
				}
				case ZED_KEY_BRACKETLEFT:
				{
					return "BRACKET LEFT";
				}
				case ZED_KEY_BRACKETRIGHT:
				{
					return "BRACKET RIGHT";
				}
				case ZED_KEY_SEMICOLON:
				{
					return "SEMICOLON";
				}
				case ZED_KEY_SINGLEQUOTE:
				{
					return "SINGLE QUOTE";
				}
				case ZED_KEY_BACKTICK:
				{
					return "BACKTICK";
				}
				case ZED_KEY_BACKSLASH:
				{
					return "BACKSLASH";
				}
				case ZED_KEY_COMMA:
				{
					return "COMMA";
				}
				case ZED_KEY_PERIOD:
				{
					return "PERIOD";
				}
				case ZED_KEY_FORWARDSLASH:
				{
					return "FORWARDSLASH";
				}
				case ZED_KEY_1:
				{
					return "1";
				}
				case ZED_KEY_2:
				{
					return "2";
				}
				case ZED_KEY_3:
				{
					return "3";
				}
				case ZED_KEY_4:
				{
					return "4";
				}
				case ZED_KEY_5:
				{
					return "5";
				}
				case ZED_KEY_6:
				{
					return "6";
				}
				case ZED_KEY_7:
				{
					return "7";
				}
				case ZED_KEY_8:
				{
					return "8";
				}
				case ZED_KEY_9:
				{
					return "9";
				}
				case ZED_KEY_0:
				{
					return "0";
				}
				case ZED_KEY_BACKSPACE:
				{
					return "BACKSPACE";
				}
				case ZED_KEY_COMMAND:
				{
					return "COMMAND";
				}
				case ZED_KEY_CAPSLOCK:
				{
					return "CAPS LOCK";
				}
				case ZED_KEY_SCROLLLOCK:
				{
					return "SCROLL LOCK";
				}
				case ZED_KEY_POWER:
				{
					return "POWER";
				}
				case ZED_KEY_PAUSE:
				{
					return "PAUSE";
				}
				case ZED_KEY_UPARROW:
				{
					return "UP ARROW";
				}
				case ZED_KEY_DOWNARROW:
				{
					return "DOWN ARROW";
				}
				case ZED_KEY_LEFTARROW:
				{
					return "LEFT ARROW";
				}
				case ZED_KEY_RIGHTARROW:
				{
					return "RIGHT ARROW";
				}
				case ZED_KEY_LSUPER:
				{
					return "LEFT SUPER";
				}
				case ZED_KEY_RSUPER:
				{
					return "RIGHT SUPER";
				}
				case ZED_KEY_SUPER:
				{
					return "SUPER";
				}
				case ZED_KEY_ALT:
				{
					return "ALT";
				}
				case ZED_KEY_CTRL:
				{
					return "CONTROL";
				}
				case ZED_KEY_SHIFT:
				{
					return "SHIFT";
				}
				case ZED_KEY_INS:
				{
					return "INSERT";
				}
				case ZED_KEY_DEL:
				{
					return "DELETE";
				}
				case ZED_KEY_PGDN:
				{
					return "PAGE DOWN";
				}
				case ZED_KEY_PGUP:
				{
					return "PAGE UP";
				}
				case ZED_KEY_HOME:
				{
					return "HOME";
				}
				case ZED_KEY_END:
				{
					return "END";
				}
				case ZED_KEY_F1:
				{
					return "F1";
				}
				case ZED_KEY_F2:
				{
					return "F2";
				}
				case ZED_KEY_F3:
				{
					return "F3";
				}
				case ZED_KEY_F4:
				{
					return "F4";
				}
				case ZED_KEY_F5:
				{
					return "F5";
				}
				case ZED_KEY_F6:
				{
					return "F6";
				}
				case ZED_KEY_F7:
				{
					return "F7";
				}
				case ZED_KEY_F8:
				{
					return "F8";
				}
				case ZED_KEY_F9:
				{
					return "F9";
				}
				case ZED_KEY_F10:
				{
					return "F10";
				}
				case ZED_KEY_F11:
				{
					return "F11";
				}
				case ZED_KEY_F12:
				{
					return "F12";
				}
				case ZED_KEY_INVERSE_EXCLAMATION:
				{
					return "INVERSE EXCLAMATION";
				}
				case ZED_KEY_F15:
				{
					return "F15";
				}
				case ZED_KEY_NP_HOME:
				{
					return "NP HOME";
				}
				case ZED_KEY_NP_UPARROW:
				{
					return "NP UP ARROW";
				}
				case ZED_KEY_NP_PGUP:
				{
					return "NP PAGE UP";
				}
				case ZED_KEY_NP_LEFTARROW:
				{
					return "NP LEFT ARROW";
				}
				case ZED_KEY_NP_5:
				{
					return "NP 5";
				}
				case ZED_KEY_NP_RIGHTARROW:
				{
					return "NP RIGHT ARROW";
				}
				case ZED_KEY_NP_END:
				{
					return "NP KP END";
				}
				case ZED_KEY_NP_DOWNARROW:
				{
					return "NP DOWN ARROW";
				}
				case ZED_KEY_NP_PGDN:
				{
					return "NP PAGE DOWN";
				}
				case ZED_KEY_NP_ENTER:
				{
					return "NP ENTER";
				}
				case ZED_KEY_NP_INS:
				{
					return "NP INSERT";
				}
				case ZED_KEY_NP_DEL:
				{
					return "NP DELETE";
				}
				case ZED_KEY_NP_SLASH:
				{
					return "NP SLASH";
				}
				case ZED_KEY_SUPERSCRIPT_TWO:
				{
					return "SUPERSCRIPT TWO";
				}
				case ZED_KEY_NP_MINUS:
				{
					return "NP MINUS";
				}
				case ZED_KEY_ACUTE_ACCENT:
				{
					return "ACUTE ACCENT";
				}
				case ZED_KEY_NP_PLUS:
				{
					return "NP PLUS";
				}
				case ZED_KEY_NP_NUMLOCK:
				{
					return "NP NUMBER LOCK";
				}
				case ZED_KEY_NP_ASTERISK:
				{
					return "NP ASTERISK";
				}
				case ZED_KEY_NP_EQUALS:
				{
					return "NP EQUALS";
				}
				default:
				{
					return "UNKNOWN";
				}
			}
		}

		const ZED_CHAR8 *MouseAxisToString( const ZED_MOUSE_AXIS p_Axis )
		{
			switch( p_Axis )
			{
				case ZED_MOUSE_AXIS_X:
				{
					return "MOUSE AXIS X";
				}
				case ZED_MOUSE_AXIS_Y:
				{
					return "MOUSE AXIS Y";
				}
				default:
				{
					return "UNKNOWN";
				}
			}
		}
	}
}

