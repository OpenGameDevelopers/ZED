#include <Windows.h>

int __stdcall WinMain( HINSTANCE p_ThisInst, HINSTANCE p_PrevInst,
	LPSTR p_CmdLine, int p_CmdShow )
{
	::MessageBoxA( NULL, "Hey", "HeyBox", MB_OK );
	return 0;
}
