#include <curses.h>

int main( int p_Argc, char *p_ppArgv[ ] )
{
	// Start up CURSES
	initscr( );

	resize_term( 25, 80 );

	printw( "Hello, Foundation with CURSES" );

	refresh( );

	getch( );
	// Clean up CURSES
	endwin( );

	return 0;
}
