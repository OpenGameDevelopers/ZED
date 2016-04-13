#include <System/WindowX11.hpp>
#include <System/WindowX11Data.hpp>
#include <X11/extensions/Xrandr.h>
#include <Renderer/Renderer.hpp>
#include <System/Debugger.hpp>
#include <System/Memory.hpp>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <vector>

#define MWM_HINTS_FUNCTIONS		( 1L << 0 )
#define MWM_HINTS_DECORATIONS	( 1L << 1 )
#define MWM_HINTS_INPUT_MODE	( 1L << 2 )
#define MWM_HINTS_STATUS		( 1L << 3 )

#define MWM_FUNC_NONE		0L
#define MWM_FUNC_ALL		( 1L << 0 )
#define MWM_FUNC_RESIZE		( 1L << 1 )
#define MWM_FUNC_MOVE		( 1L << 2 )
#define MWM_FUNC_MINIMISE	( 1L << 3 )
#define MWM_FUNC_MAXIMISE	( 1L << 4 )
#define MWM_FUNC_CLOSE		( 1L << 5 )

#define MWM_DECOR_NONE		0L
#define MWM_DECOR_ALL		( 1L << 0 )
#define MWM_DECOR_BORDER	( 1L << 1 )
#define MWM_DECOR_RESIZEH	( 1L << 2 )
#define MWM_DECOR_TITLE		( 1L << 3 )
#define MWM_DECOR_MENU		( 1L << 4 )
#define MWM_DECOR_MINIMISE	( 1L << 5 )
#define MWM_DECOR_MAXIMISE	( 1L << 6 )

namespace ZED
{
	namespace System
	{
		int g_XrandrMajor = 0, g_XrandrMinor = 0;
		ZED_BOOL g_XrandrSupported = ZED_FALSE;
		ZED_BOOL g_XrandrInitialised = ZED_FALSE;
		XRRScreenResources *g_pXRRScreenResources;

		enum XRANDR_NAME_KIND
		{
			XRANDR_NAME_KIND_NONE = 0,
			XRANDR_NAME_KIND_STRING = 1,
			XRANDR_NAME_KIND_XID = 2,
			XRANDR_NAME_KIND_INDEX = 4,
			XRANDR_NAME_KIND_PREFERRED = 8
		};

		struct XRANDR_NAME
		{
			XRANDR_NAME_KIND	Kind;
			char				*pString;
			XID					ID;
			int					Index;
		};

		ZED_UINT32 InitialiseXrandr( Display *p_pDisplay )
		{
			// Regardless of whether the function succeeds, Xrandr has been
			// initialised
			g_XrandrInitialised = ZED_TRUE;

			if( !XRRQueryVersion( p_pDisplay, &g_XrandrMajor,
				&g_XrandrMinor ) )
			{
				zedTrace( "[ZED::System::InitialiseXrandr] <ERROR> "
					"Xrandr not found\n" );

				return ZED_FAIL;
			}

			g_XrandrSupported = ZED_TRUE;

			zedTrace( "[ZED::System::InitialiseXrandr] <INFO> "
				"Version %d.%d of Xrandr found\n", g_XrandrMajor,
				g_XrandrMinor );

			return ZED_OK;
		}

		int TranslateXrandrScreenToReal( int p_DisplayNumber,
			int p_ScreenNumber )
		{
			return 0;
		}

		XRRModeInfo *GetModeInfoByID( RRMode p_Mode )
		{

		}

		ZED_UINT32 GetNativeScreenSize( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber,
			SCREEN &p_ScreenSize )
		{
			ZED_UINT32 ScreenCount = 0;
			GetScreenCount( p_DisplayNumber, &ScreenCount );

			zedTrace( "Got %d screens\n", ScreenCount );

			if( ( p_ScreenNumber < 0 ) ||
				( p_ScreenNumber > ( ScreenCount-1 ) ) )
			{
				zedTrace( "[ZED::Renderer::GetNativeScreenSize] <ERROR> "
					"Screen count %s than what is available\n",
					( p_ScreenNumber < 0 ) ? "less" : "more" );
			}

			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetNativeScreenSize] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			if( g_XrandrInitialised == ZED_FALSE )
			{
				InitialiseXrandr( pDisplay );
			}

			if( g_XrandrSupported == ZED_TRUE )
			{
				g_pXRRScreenResources = XRRGetScreenResourcesCurrent( pDisplay,
					RootWindow( pDisplay, 0 ) );

				/*XRROutputInfo *pOutputInfo = XRRGetOutputInfo( pDisplay,
					g_pXRRScreenResources,
					g_pXRRScreenResources->outputs[ 3 ] );*/
				zedTrace( "ncrtc: %d\n", g_pXRRScreenResources->ncrtc );
				XRRCrtcInfo *pCRTCInfo = XRRGetCrtcInfo( pDisplay,
					g_pXRRScreenResources,
					g_pXRRScreenResources->crtcs[ p_ScreenNumber ] );
				zedTrace( "X: %d\n",  pCRTCInfo->x );

				p_ScreenSize.Width = pCRTCInfo->width;
				p_ScreenSize.Height = pCRTCInfo->height;
				p_ScreenSize.X = pCRTCInfo->x;
				p_ScreenSize.Y = pCRTCInfo->y;

				zedTrace( "Screen size: %dx%d\n", p_ScreenSize.Width,
					p_ScreenSize.Height );
			}
			else
			{
				p_ScreenSize.Width = DisplayWidth( pDisplay, p_ScreenNumber );
				p_ScreenSize.Height = DisplayHeight( pDisplay,
					p_ScreenNumber );
				p_ScreenSize.X = 0;
				p_ScreenSize.Y = 0;
			}

			XCloseDisplay( pDisplay );

			return ZED_OK;
		}

		ZED_UINT32 GetDisplayCount( ZED_UINT32 *p_pDisplayCount )
		{
			DIR *pXDirectory = opendir( "/tmp/.X11-unix" );

			if( pXDirectory == ZED_NULL )
			{
				zedTrace( "[ZED::System::GetDisplayCount] <ERROR> "
					"Failed to enumerate displays, could not find directory: "
					"\"/tmp/.X11-unix\"\n");

				return ZED_FAIL;
			}
			
			struct dirent *pXEntry;

			while( ( pXEntry = readdir( pXDirectory ) ) != ZED_NULL )
			{
				if( pXEntry->d_name[ 0 ] != 'X' )
				{
					continue;
				}

				++( *p_pDisplayCount );
			}

			closedir( pXDirectory );

			return ZED_OK;
		}

		ZED_UINT32 GetScreenCount( const ZED_UINT32 p_DisplayNumber,
			ZED_UINT32 *p_pScreenCount )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

			if( pDisplay == ZED_NULL )
			{
				( *p_pScreenCount ) = 0;
				
				zedTrace( "[ZED::Renderer::GetScreenCount] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			if( g_XrandrInitialised == ZED_FALSE )
			{
				InitialiseXrandr( pDisplay );
			}

			ZED_UINT32 ScreenCount = 0;

			if( g_XrandrSupported == ZED_TRUE )
			{
				if( ( g_XrandrMajor == 1 ) && ( g_XrandrMinor < 2 ) )
				{
					zedTrace( "[ZED::Renderer::GetScreenCount] <ERROR> "
						"Xrandr version is less than 1.2, cannot get screen "
						"count from Xrandr\n" );

					ScreenCount = ScreenCount( pDisplay );
				}
				else
				{
					int GrossScreens = 0;

					for( int Index = 0; Index < ScreenCount( pDisplay );
						++Index )
					{
						if( ( g_XrandrMajor == 1 ) && ( g_XrandrMinor >= 3 ) )
						{
							g_pXRRScreenResources =
								XRRGetScreenResourcesCurrent( pDisplay,
									RootWindow( pDisplay, Index ) );
						}
						else
						{
							g_pXRRScreenResources =
								XRRGetScreenResources( pDisplay,
									RootWindow( pDisplay, Index ) );
						}

						GrossScreens += g_pXRRScreenResources->noutput;
					}

					ScreenCount = GrossScreens;

					zedTrace( "Gross screen count: %d\n", GrossScreens );
				}
			}
			else
			{
				ScreenCount = ScreenCount( pDisplay );
			}

			XCloseDisplay( pDisplay );

			( *p_pScreenCount ) = ScreenCount;

			zedTrace( "ScreenCount: %d\n", ScreenCount );

			return ZED_OK;
		}

		SCREEN_ORIENTATION GetScreenOrientation(
			const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetScreenOrientation] <ERROR> "
					"Could not open display\n" );

				return SCREEN_ORIENTATION_0;
			}

			SCREEN_ORIENTATION Orientation = SCREEN_ORIENTATION_0;
			
			Rotation CurrentRotation;
			XRRRotations( pDisplay, p_ScreenNumber, &CurrentRotation );

			if( CurrentRotation == RR_Rotate_0 )
			{
				Orientation = SCREEN_ORIENTATION_0;
			}
			if( CurrentRotation == RR_Rotate_90 )
			{
				Orientation = SCREEN_ORIENTATION_90;
			}
			if( CurrentRotation == RR_Rotate_180 )
			{
				Orientation = SCREEN_ORIENTATION_180;
			}
			if( CurrentRotation == RR_Rotate_270 )
			{
				Orientation = SCREEN_ORIENTATION_270;
			}

			return Orientation;
		}

		ZED_UINT32 EnumerateScreens( const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber, SCREEN **p_ppSizes,
			ZED_MEMSIZE *p_pCount )
		{
			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			sprintf( pDisplayNumber, ":%d", p_DisplayNumber );

			Display *pDisplay = XOpenDisplay( pDisplayNumber );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::EnumerateScreenSizes] <ERROR> "
					"Could not open display\n" );

				return ZED_FAIL;
			}

			if( g_XrandrInitialised == ZED_FALSE )
			{
				InitialiseXrandr( pDisplay );
			}


			g_pXRRScreenResources = XRRGetScreenResourcesCurrent( pDisplay,
				RootWindow( pDisplay, 0) );

			zedTrace( "Output count: %d\n", g_pXRRScreenResources->noutput );
			zedTrace( "Screen number: %u\n", p_ScreenNumber );
			XRROutputInfo *pOutputInfo = XRRGetOutputInfo( pDisplay,
				g_pXRRScreenResources,
				g_pXRRScreenResources->outputs[ 2 ] );

			ZED_SINT32 TotalSizes = pOutputInfo->nmode;
			zedTrace( "Found %d modes\n", pOutputInfo->nmode );
			zedTrace( "Found %d modes\n", g_pXRRScreenResources->nmode );

			//( *p_ppSizes ) = new SCREEN[ TotalSizes ];
			std::vector< SCREEN > Screens;

			for( int i = 0; i < pOutputInfo->nmode; ++i )
			{
				RRMode Mode = pOutputInfo->modes[ i ];
				XRRModeInfo *pMode = ZED_NULL;
				zedTrace( "Getting mode\n" );
				zedTrace( "CRTC count: %d\n", pOutputInfo->ncrtc );

				Rotation XRotation = RR_Rotate_0;

				for( int j = 0; j < g_pXRRScreenResources->nmode; ++j )
				{
					pMode = &g_pXRRScreenResources->modes[ j ];
					if( Mode == pMode->id )
					{
						break;
					}
					pMode = ZED_NULL;
				}

				ZED_SINT32 XOffset = 0, YOffset = 0;

				for( int j = 0; j < pOutputInfo->ncrtc; ++j )
				{
					XRRCrtcInfo *pCRTC = XRRGetCrtcInfo( pDisplay,
						g_pXRRScreenResources,
						g_pXRRScreenResources->crtcs[ j ] );

					for( int k = 0; k < g_pXRRScreenResources->ncrtc; ++k )
					{
						if( pOutputInfo->crtcs[ k ] == g_pXRRScreenResources->crtcs[ j ] )
						{
							zedTrace( "GOT CRTC\n" );

							zedTrace( "CRTC X: %d\n", pCRTC->x );
							zedTrace( "Rotation: %d\n", pCRTC->rotation );
						}
					}
				}

				if( pMode )
				{
					ZED_UINT32 Width = 0;
					ZED_UINT32 Height = 0;
					std::string ModeString( pMode->name );
					ZED_MEMSIZE TokenPosition =
						ModeString.find_first_of( "x" );
					std::string WidthString = ModeString.substr( 0,
						TokenPosition );
					std::string HeightString =
						ModeString.substr( TokenPosition + 1 );
					Width = strtoul( WidthString.c_str( ), ZED_NULL, 10 );
					Height = strtoul( HeightString.c_str( ), ZED_NULL, 10 );
					zedTrace( "Got mode\n" );
					zedTrace( "Mode %d: %ux%u", i, Width, Height );
					ZED_BOOL Duplicate = ZED_FALSE;
					for( int j = 0; j < Screens.size( ); ++j )
					{
						if( Screens[ j ].Width == Width &&
							Screens[ j ].Height == Height )
						{
							Duplicate = ZED_TRUE;
						}
					}

					if( Duplicate == ZED_TRUE )
					{
						zedTrace( " DUPLICATE" );
					}
					else
					{
						SCREEN NewScreen;
						NewScreen.Width = Width;
						NewScreen.Height = Height;
						Screens.push_back( NewScreen );
					}

					zedTrace( "\n" );
				}

				/*XRRModeInfo *pMode;

				zedTrace( "\t%d: %dx%d\n",
					i, pScreenSize[ i ].width, pScreenSize[ i ].height );

				( *p_ppSizes )[ i ].Width = pScreenSize[ i ].width;
				( *p_ppSizes )[ i ].Height = pScreenSize[ i ].height;*/
			}

			( *p_ppSizes ) = new SCREEN[ Screens.size( ) ];

			for( ZED_MEMSIZE i = 0; i < Screens.size( ); ++i )
			{
				( *p_ppSizes )[ i ].Width = Screens[ i ].Width;
				( *p_ppSizes )[ i ].Height = Screens[ i ].Height;
			}

			return ZED_FAIL;
			/*XRRScreenSize *pScreenSize = XRRSizes( pDisplay, p_ScreenNumber,
				&TotalSizes );*/
			XCloseDisplay( pDisplay );

			zedTrace( "[ZED::Renderer::EnumerateScreenSizes] <INFO> "
				"Found %d resolutions\n", TotalSizes );


			( *p_pCount ) = TotalSizes;
			
			return ZED_OK;
		}

		ZED_SINT32 GetCurrentDisplayNumber( )
		{
			char *pDisplayNumber = getenv( "DISPLAY" );

			if( pDisplayNumber )
			{
				ZED_MEMSIZE DisplayLength = strlen( pDisplayNumber );
				char Current = pDisplayNumber[ 0 ];
				char DisplayString[ DisplayLength + 1 ];
				memset( DisplayString, '\0', ( DisplayLength + 1 ) *
					sizeof( char ) );
				ZED_MEMSIZE Counter = 0;

				for( ZED_MEMSIZE i = 0; i < DisplayLength; ++i )
				{
					Current = pDisplayNumber[ i ];

					if( Current == ':' )
					{
						++i;
						continue;
					}
					if( Current == '.' )
					{
						DisplayString[ Counter ] = Current;
						++Counter;
						break;
					}
				}

				ZED_SINT32 DisplayNumber = strtol( DisplayString, ZED_NULL,
					10 );

				return DisplayNumber;
			}

			return -1;
		}

		ZED_SINT32 GetCurrentScreenNumber( )
		{
			char *pDisplayNumber = getenv( "DISPLAY" );

			if( pDisplayNumber )
			{
				if( g_XrandrInitialised == ZED_FALSE )
				{
					char pDisplayString[ 16 ];
					sprintf( pDisplayString, ":%d",
						GetCurrentDisplayNumber( ) );
					Display *pDisplay = XOpenDisplay( pDisplayString );
					InitialiseXrandr( pDisplay );
				}
				if( g_XrandrSupported == ZED_FALSE )
				{
					ZED_MEMSIZE DisplayLength = strlen( pDisplayNumber );
					ZED_CHAR8 ScreenString[ DisplayLength ];
					strncpy( ScreenString, pDisplayNumber, DisplayLength );
					ZED_CHAR8 ScreenNumber[ DisplayLength + 1 ];
					memset( ScreenNumber, '\0',
						sizeof( ZED_CHAR8 ) * ( DisplayLength + 1 ) );
					ZED_MEMSIZE ScreenIterator = 0;

					for( ZED_MEMSIZE i = DisplayLength - 1; i > 0; --i )
					{
						if( ScreenString[ i ] == '.' )
						{
							break;
						}

						ScreenNumber[ ScreenIterator ] = ScreenString[ i ];
						++ScreenIterator;
					}

					ZED_SINT32 CurrentScreenNumber = strtol( ScreenNumber,
						ZED_NULL, 10 );

					return CurrentScreenNumber;
				}
				else
				{
					char pDisplayString[ 16 ];
					sprintf( pDisplayString, ":%d",
						GetCurrentDisplayNumber( ) );
					Display *pDisplay = XOpenDisplay( pDisplayString );
					int MouseX = 0, MouseY = 0;
					int WinX = 0, WinY = 0;
					::Window RootWin, ChildWin;
					unsigned int Mask = 0;

					for( int Index = 0; Index < ScreenCount( pDisplay );
						++Index )
					{
						if( XQueryPointer( pDisplay,
							RootWindow( pDisplay, Index ), &RootWin,
							&ChildWin, &MouseX, &MouseY, &WinX, &WinY,
							&Mask ) == False )
						{
							continue;
						}

						zedTrace( "Mouse position: %d %d\n", MouseX, MouseY );
						zedTrace( "Mouse position: %d %d\n", WinX, WinY );

						if( ( g_XrandrMajor == 1 ) && ( g_XrandrMinor >= 3 ) )
						{
							g_pXRRScreenResources =
								XRRGetScreenResourcesCurrent( pDisplay,
									RootWindow( pDisplay, Index ) );
						}
						else
						{
							g_pXRRScreenResources =
								XRRGetScreenResources( pDisplay,
									RootWindow( pDisplay, Index ) );
						}

						// Find the screen which the pointer is within

				/*zedTrace( "ncrtc: %d\n", g_pXRRScreenResources->ncrtc );
				XRRCrtcInfo *pCRTCInfo = XRRGetCrtcInfo( pDisplay,
					g_pXRRScreenResources,
					g_pXRRScreenResources->crtcs[ p_ScreenNumber ] );*/

				/*g_pXRRScreenResources = XRRGetScreenResourcesCurrent( pDisplay,
					RootWindow( pDisplay, 0) );
				XRROutputInfo *pOutputInfo = XRRGetOutputInfo( pDisplay,
					g_pXRRScreenResources,
					g_pXRRScreenResources->outputs[ 2 ] );*/
						for( int ScreenIndex = 0; ScreenIndex <
							g_pXRRScreenResources->ncrtc; ++ScreenIndex )
						{
							XRRCrtcInfo *pCRTCInfo = XRRGetCrtcInfo( pDisplay,
								g_pXRRScreenResources,
								g_pXRRScreenResources->crtcs[ ScreenIndex ] );
							
							if( ( MouseX >= pCRTCInfo->x &&
									( MouseX <= ( pCRTCInfo->x +
										pCRTCInfo->width ) ) )  &&
								( MouseY >= pCRTCInfo->y &&
									( MouseY <= ( pCRTCInfo->y +
										pCRTCInfo->height ) ) ) )
							{
								zedTrace( "Mouse is in %d\n", ScreenIndex );

								return ScreenIndex;
							}
						}
					}
				}
			}

			return -1;
		}

		SCREEN_ORIENTATION GetCurrentScreenOrientation( )
		{
			const ZED_UINT32 ScreenNumber = GetCurrentScreenNumber( );
			Display *pDisplay = XOpenDisplay( ZED_NULL );

			if( pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::GetScreenOrientation] <ERROR> "
					"Could not open display\n" );

				return SCREEN_ORIENTATION_0;
			}

			SCREEN_ORIENTATION Orientation = SCREEN_ORIENTATION_0;
			
			Rotation CurrentRotation;
			XRRRotations( pDisplay, ScreenNumber, &CurrentRotation );

			if( CurrentRotation == RR_Rotate_0 )
			{
				Orientation = SCREEN_ORIENTATION_0;
			}
			if( CurrentRotation == RR_Rotate_90 )
			{
				Orientation = SCREEN_ORIENTATION_90;
			}
			if( CurrentRotation == RR_Rotate_180 )
			{
				Orientation = SCREEN_ORIENTATION_180;
			}
			if( CurrentRotation == RR_Rotate_270 )
			{
				Orientation = SCREEN_ORIENTATION_270;
			}

			return Orientation;
		}

		LinuxWindow::LinuxWindow( ) :
			m_pXVisualInfo( ZED_NULL ),
			m_pScreen( ZED_NULL ),
			m_pWindowData( ZED_NULL ),
			m_Window( 0 ),
			m_pDisplay( ZED_NULL ),
			m_CursorHidden( ZED_FALSE ),
			m_FullScreen( ZED_FALSE ),
			m_X( 0 ),
			m_Y( 0 ),
			m_Width( 0 ),
			m_Height( 0 ),
			m_Running( ZED_FALSE ),
			m_MouseGrabbed( ZED_FALSE ),
			m_ConfineMouse( ZED_FALSE ),
			m_MouseCentred( ZED_FALSE )
		{
			m_DisplayNumber = 0;
			m_ScreenNumber = 0;
			m_Style = 0;
			m_pTitle = ZED_NULL;
		}

		LinuxWindow::LinuxWindow( const Window &p_RawWindow )
		{
			memcpy( this, &p_RawWindow, sizeof( LinuxWindow ) );
		}

		LinuxWindow::~LinuxWindow( )
		{
			if( m_CursorHidden )
			{
				this->ShowCursor( );
			}

			this->Destroy( );
		}

		ZED_UINT32 LinuxWindow::Create( const ZED_UINT32 p_X,
			const ZED_UINT32 p_Y, const ZED_UINT32 p_Width,
			const ZED_UINT32 p_Height, const ZED_UINT32 p_DisplayNumber,
			const ZED_UINT32 p_ScreenNumber, const ZED_UINT32 p_Style )
		{
			m_Running = ZED_FALSE;

			char pDisplayNumber[ 16 ];
			memset( pDisplayNumber, '\0', sizeof( char )*16 );
			int RealScreenNumber = TranslateXrandrScreenToReal(
				p_DisplayNumber, p_ScreenNumber );
			sprintf( pDisplayNumber, ":%d.%d", p_DisplayNumber,
				RealScreenNumber );

			zedTrace( "[ZED::System::LinuxWindow::Create] <INFO> Creating "
				"a window at %s\n", pDisplayNumber );

			if( m_pDisplay == ZED_NULL )
			{
				m_pDisplay = XOpenDisplay( pDisplayNumber );
			}

			if( m_pDisplay == ZED_NULL )
			{
				zedTrace( "[ZED::Renderer::LinuxWindow::Create] <ERROR> "
					"Could not open display :%d.%d\n", p_DisplayNumber,
						RealScreenNumber );
				return ZED_FAIL;
			}

			m_pScreen = XScreenOfDisplay( m_pDisplay, RealScreenNumber );

			XSetWindowAttributes WinAttribs;
			WinAttribs.background_pixmap = None;
			WinAttribs.border_pixel = 0;
			WinAttribs.event_mask = StructureNotifyMask | ExposureMask |
				KeyPressMask | KeyReleaseMask |
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
				FocusChangeMask | EnterWindowMask | LeaveWindowMask;
			WinAttribs.override_redirect = False;

			if( p_Style == ZED_WINDOW_STYLE_FULLSCREEN )
			{
				Atom State = XInternAtom( m_pDisplay, "_NET_WM_STATE", False );
				Atom Fullscreen = XInternAtom( m_pDisplay,
					"_NET_WM_STATE_FULLSCREEN", False );

				XEvent Event;
				memset( &Event, 0, sizeof( Event ) );

				Event.type = ClientMessage;
				Event.xclient.window		= m_Window;
				Event.xclient.message_type	= State;
				Event.xclient.format		= 32;
				Event.xclient.data.l[ 0 ]	= 1;
				Event.xclient.data.l[ 1 ]	= Fullscreen;

				XSendEvent( m_pDisplay, DefaultRootWindow( m_pDisplay ),
					False, SubstructureNotifyMask, &Event );

				SCREEN NativeSize;
				GetNativeScreenSize( p_DisplayNumber, p_ScreenNumber,
					NativeSize );
				m_X = NativeSize.X;
				m_Y = NativeSize.Y;
				m_Width = NativeSize.Width;
				m_Height = NativeSize.Height;
			}
			else
			{
				m_X = p_X;
				m_Y = p_Y;
				m_Width = p_Width;
				m_Height = p_Height;
			}

			int WindowDepth = CopyFromParent;
			Visual *pVisual = CopyFromParent;
			unsigned long ValueMask = CWEventMask | CWBorderPixel;

			if( m_pXVisualInfo )
			{
				XSync( m_pDisplay, True );

				WindowDepth = m_pXVisualInfo->depth;
				pVisual = m_pXVisualInfo->visual;
				ValueMask |= CWColormap;

				WinAttribs.colormap = XCreateColormap( m_pDisplay,
					RootWindow( m_pDisplay, m_pXVisualInfo->screen ),
					pVisual, AllocNone );
			}

			zedTrace( "Real screen num: %d\n", RealScreenNumber );
			zedTrace( "pScreennum: %d\n", p_ScreenNumber );

			m_Window = XCreateWindow( m_pDisplay,
				RootWindow( m_pDisplay, RealScreenNumber ),
				m_X, m_Y, m_Width, m_Height, 0, WindowDepth, InputOutput,
				pVisual, ValueMask, &WinAttribs );
			zedTrace( "XCreateWin done\n" );

			zedTrace( "X: %d\nY: %d\nWidth: %d\nHeight: %d\n",
				m_X, m_Y, m_Width, m_Height );

			zedSafeDelete( m_pWindowData );

			m_pWindowData = new LinuxWindowData( m_pDisplay, m_Window,
				m_pScreen );

			zedTrace( "bef\n" );
			XSync( m_pDisplay, True );
			zedTrace( "aft\n" );

			Atom DeleteMessage = XInternAtom( m_pDisplay, "WM_DELETE_WINDOW",
				False );
			XSetWMProtocols( m_pDisplay, m_Window, &DeleteMessage, 1 );

			Atom Property = XInternAtom( m_pDisplay, "_MOTIF_WM_HINTS",
				False );

			if( Property )
			{
				typedef struct __tagHints
				{
					unsigned long	Flags;
					unsigned long	Functions;
					unsigned long	Decorations;
					long			InputMode;
					unsigned long	State;
				} Hints;

				Hints WindowDecor;
				WindowDecor.Flags = MWM_HINTS_FUNCTIONS |
					MWM_HINTS_DECORATIONS;
				WindowDecor.Functions = 0L;
				WindowDecor.Decorations = 0L;

				if( ( p_Style & ZED_WINDOW_STYLE_FULLSCREEN ) ||
					( p_Style & ZED_WINDOW_STYLE_NONE ) )
				{
					WindowDecor.Functions |= MWM_FUNC_NONE;
					WindowDecor.Decorations |= MWM_DECOR_NONE;
				}
				else if( p_Style == ZED_WINDOW_STYLE_ALL )
				{
					WindowDecor.Functions |= MWM_FUNC_ALL;
					WindowDecor.Decorations |= MWM_DECOR_ALL;
				}
				else
				{
					if( p_Style & ZED_WINDOW_STYLE_CLOSE )
					{
						WindowDecor.Functions |= MWM_FUNC_CLOSE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MINIMISE )
					{
						WindowDecor.Functions |= MWM_FUNC_MINIMISE;
						WindowDecor.Decorations |= MWM_DECOR_MINIMISE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MAXIMISE )
					{
						WindowDecor.Functions |= MWM_FUNC_MAXIMISE;
						WindowDecor.Decorations |= MWM_DECOR_MAXIMISE;
					}

					if( p_Style & ZED_WINDOW_STYLE_RESIZE )
					{
						WindowDecor.Functions |= MWM_FUNC_RESIZE;
						WindowDecor.Decorations |= MWM_DECOR_RESIZEH;
					}

					if( p_Style & ZED_WINDOW_STYLE_TITLEBAR )
					{
						WindowDecor.Decorations |= MWM_DECOR_TITLE;
					}

					if( p_Style & ZED_WINDOW_STYLE_MENU )
					{
						WindowDecor.Decorations |= MWM_DECOR_MENU;
					}
					
					if( p_Style & ZED_WINDOW_STYLE_BORDER )
					{
						WindowDecor.Decorations |= MWM_DECOR_BORDER;
					}

					if( p_Style & ZED_WINDOW_STYLE_MOVE )
					{
						WindowDecor.Functions |= MWM_FUNC_MOVE;
					}
				}
				
				XChangeProperty( m_pDisplay, m_Window, Property, Property, 32,
					PropModeReplace, ( unsigned char *)&WindowDecor, 5 );

				// Kind of an inelegant way to disable window re-sizing
				if( !( p_Style & ZED_WINDOW_STYLE_RESIZE ) )
				{
					XSizeHints *pSizeHints = XAllocSizeHints( );
					pSizeHints->flags = PMinSize | PMaxSize;
					pSizeHints->min_width = m_Width;
					pSizeHints->min_height = m_Height;
					pSizeHints->max_width = m_Width;
					pSizeHints->max_height = m_Height;

					XSetWMNormalHints( m_pDisplay, m_Window, pSizeHints );

					XFree( pSizeHints );
				}
			}
			else
			{
				zedTrace( "[ZED::LinuxWindow::Create] <WARNING> "
					"Could not acquire the property: \"_MOTIF_WM_HINTS\"\n" );
			}

			XMapWindow( m_pDisplay, m_Window );
			XMapRaised( m_pDisplay, m_Window );
			if( p_Style & ZED_WINDOW_STYLE_CENTRED )
			{
				SCREEN DisplayScreen;
				GetNativeScreenSize( p_DisplayNumber, p_ScreenNumber,
					DisplayScreen );
				ZED_UINT32 X = ( DisplayScreen.Width / 2 ) -
					( p_Width / 2 );
				ZED_UINT32 Y = ( DisplayScreen.Height / 2 ) -
					( p_Height / 2 );
				m_X = X;
				m_Y = Y;
				XMoveWindow( m_pDisplay, m_Window, X, Y );
			}
			else
			{
				XMoveWindow( m_pDisplay, m_Window, p_X, p_Y );
			}
			XRaiseWindow( m_pDisplay, m_Window );

			if( m_pTitle )
			{
				XStoreName( m_pDisplay, m_Window, m_pTitle );
			}

			m_Running = ZED_TRUE;
			m_Resized = ZED_FALSE;
			m_Moved = ZED_FALSE;

			m_DisplayNumber = p_DisplayNumber;
			m_ScreenNumber = p_ScreenNumber;
			m_Style = p_Style;

			return ZED_OK;
		}

		void LinuxWindow::Destroy( )
		{
			// As the data structure is memcpy'd, don't call XFree
			zedSafeDelete( m_pXVisualInfo );

			if( m_Window )
			{
				XDestroyWindow( m_pDisplay, m_Window );
			}

			if( m_pDisplay )
			{
				XCloseDisplay( m_pDisplay );
				m_pDisplay = ZED_NULL;
			}

			zedSafeDelete( m_pWindowData );
		}

		ZED_UINT32 LinuxWindow::Update( )
		{
			XEvent Event;

			int Pending = XEventsQueued( m_pDisplay, QueuedAfterReading );

			XEvent QueuedEvents[ Pending ];
			memset( &QueuedEvents, 0, sizeof( XEvent ) * Pending );
			int Resend = 0;

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pDisplay, &Event );
				switch( Event.type )
				{
					case ClientMessage:
					{
						if( *XGetAtomName( m_pDisplay,
							Event.xclient.message_type ) == *"WM_PROTOCOLS" )
						{
							m_Running = ZED_FALSE;
						}
						break;
					}
					case ConfigureNotify:
					{
						XConfigureEvent ConfigureEvent = Event.xconfigure;

						if( ( m_X != ConfigureEvent.x ) ||
							( m_Y != ConfigureEvent.y ) )
						{
							m_X = ConfigureEvent.x;
							m_Y = ConfigureEvent.y;
							m_Moved = ZED_TRUE;
						}

						if( ( m_Width != ConfigureEvent.width ) ||
							( m_Height != ConfigureEvent.height ) )
						{
							m_Width = ConfigureEvent.width;
							m_Height = ConfigureEvent.height;
							m_Resized = ZED_TRUE;
						}
						break;
					}
					case EnterNotify:
					{
						this->GrabMouse( m_ConfineMouse, m_MouseCentred );
						this->GrabKeyboard( );
						break;
					}
					case LeaveNotify:
					{
						this->ReleaseMouse( );
						this->ReleaseKeyboard( );
						break;
					}
					default:
					{
						QueuedEvents[ Resend ] = Event;
						++Resend;
						break;
					}
				}
			}

			for( int i = Resend; i > 0; --i )
			{
				XPutBackEvent( m_pDisplay, &QueuedEvents[ i - 1 ] );
			}

			return ZED_OK;
		}

		void LinuxWindow::FlushEvents(
			const ZED_WINDOW_FLUSH_TYPE p_FlushType )
		{
			if( p_FlushType & ZED_WINDOW_FLUSH_NONE )
			{
				return;
			}
			
			XEvent Event;
			int Pending = XPending( m_pDisplay );
			XEvent QueuedEvents[ Pending ];
			memset( &QueuedEvents, 0, sizeof( XEvent ) * Pending );
			int Resend = 0;

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pDisplay, &Event );
				if( p_FlushType & ZED_WINDOW_FLUSH_ALL )
				{
					continue;
				}

				switch( Event.type )
				{
					case ButtonPress:
					case ButtonRelease:
					case MotionNotify:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_MOUSE )
						{
							continue;
						}
						break;
					}
					case KeyPress:
					case KeyRelease:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_KEYBOARD )
						{
							continue;
						}
						break;
					}
					case EnterNotify:
					case LeaveNotify:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_WINDOWCROSS )
						{
							continue;
						}
						break;
					}
					case FocusIn:
					case FocusOut:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_FOCUS )
						{
							continue;
						}
						break;
					}
					case Expose:
					case GraphicsExpose:
					case NoExpose:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_EXPOSE )
						{
							continue;
						}
						break;
					}
					case KeymapNotify:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_KEYMAP )
						{
							continue;
						}
						break;
					}
					case CirculateNotify:
					case ConfigureNotify:
					case CreateNotify:
					case DestroyNotify:
					case GravityNotify:
					case MapNotify:
					case MappingNotify:
					case ReparentNotify:
					case UnmapNotify:
					case VisibilityNotify:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_WINDOWSTATE )
						{
							continue;
						}

						break;
					}
					case ColormapNotify:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_COLOURMAP )
						{
							continue;
						}
						break;
					}
					case ClientMessage:
					case PropertyNotify:
					case SelectionClear:
					case SelectionNotify:
					case SelectionRequest:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_CLIENT )
						{
							continue;
						}

						break;
					}
					case CirculateRequest:
					case ConfigureRequest:
					case MapRequest:
					case ResizeRequest:
					{
						if( p_FlushType & ZED_WINDOW_FLUSH_STRUCTURE )
						{
							continue;
						}

						break;
					}
				}

				QueuedEvents[ Resend ] = Event;
				++Resend;
			}

			for( int i = Resend; i > 0; --i )
			{
				XPutBackEvent( m_pDisplay, &QueuedEvents[ i - 1 ] );
			}
		}


		ZED_UINT32 LinuxWindow::GetWindowData(
			WindowData **p_ppWindowData ) const
		{
			if( ( *p_ppWindowData ) == ZED_NULL )
			{
				zedTrace( "[ZED::System::LinuxWindow::GetWindowData] <ERROR> "
					"Window data parameter is null\n" );

				return ZED_FAIL;
			}

			memcpy( ( *p_ppWindowData ), m_pWindowData,
				sizeof( LinuxWindowData ) );

			return ZED_OK;
		}

		void LinuxWindow::SetTitle( const ZED_CHAR8 *p_pTitle )
		{
			if( p_pTitle )
			{
				zedSafeDeleteArray( m_pTitle );
				size_t StringLength = strlen( p_pTitle );
				m_pTitle = new ZED_CHAR8[ StringLength + 1 ];
				m_pTitle[ StringLength ] = '\0';
				strncpy( m_pTitle, p_pTitle, StringLength );
				XStoreName( m_pDisplay, m_Window, m_pTitle );
			}
		}

		void LinuxWindow::HideCursor( )
		{
			if( m_pDisplay == ZED_NULL )
			{
				return;
			}

			XDefineCursor( m_pDisplay, m_Window, this->NullCursor( ) );
		}

		void LinuxWindow::ShowCursor( )
		{
			if( m_pDisplay == ZED_NULL )
			{
				return;
			}

			XUndefineCursor( m_pDisplay, m_Window );
		}

		ZED_BOOL LinuxWindow::ToggleCursor( )
		{
			if( m_CursorHidden )
			{
				this->ShowCursor( );
			}
			else
			{
				this->HideCursor( );
			}

			m_CursorHidden = !m_CursorHidden;

			return ( m_CursorHidden != ZED_TRUE );
		}

		void LinuxWindow::SetWindowed( )
		{
		}

		void LinuxWindow::SetFullScreen( )
		{
		}

		ZED_BOOL LinuxWindow::ToggleFullScreen( )
		{
			if( m_FullScreen )
			{
				this->SetWindowed( );
			}
			else
			{
				this->SetFullScreen( );
			}

			m_FullScreen = !m_FullScreen;

			return m_FullScreen;
		}

		ZED_BOOL LinuxWindow::Closed( )
		{
			return !m_Running;
		}

		void LinuxWindow::WarpPointer( const ZED_UINT32 p_X,
			const ZED_UINT32 p_Y )
		{
			if( ( p_X > m_Width ) || ( p_Y > m_Height ) )
			{
				zedTrace( "[ZED::System::LinuxWindow::PointerPosition] <ERROR>"
					" Pointer position is larger than the width or height of "
					"the window\n" );

				return;
			}

			XWarpPointer( m_pDisplay, None, m_Window, 0, 0, 0, 0, p_X, p_Y );
		}

		ZED_UINT32 LinuxWindow::GrabKeyboard( )
		{
			int GrabStatus = 0;

			GrabStatus = XGrabKeyboard( m_pDisplay, m_Window, False,
				GrabModeAsync, GrabModeAsync, CurrentTime );

			if( GrabStatus == BadValue )
			{
				zedTrace( "[ZED::System::LinuxWindow::GrabKeyboard] <ERROR> "
					"Bad value passed to XGrabKeyboard\n" );

				return ZED_FAIL;
			}

			if( GrabStatus == BadWindow )
			{
				zedTrace( "[ZED::System::LinuxWindow::GrabKeyboard] <ERROR> "
					"Window is invalid\n" );

				return ZED_FAIL;
			}

			return ZED_OK;
		}

		ZED_UINT32 LinuxWindow::GrabMouse( const ZED_BOOL p_ConfineMouse,
			const ZED_BOOL p_CentreMouse )
		{
			m_MouseCentred = p_CentreMouse;
			if( p_CentreMouse == ZED_TRUE )
			{
				XWarpPointer( m_pDisplay, None, m_Window, 0, 0, 0, 0,
					m_Width / 2, m_Height / 2 );
				XSync( m_pDisplay, False );
			}

			int GrabStatus = 0;
			::Window ConfineMouse = None;
			m_ConfineMouse = p_ConfineMouse;

			if( m_ConfineMouse == ZED_TRUE )
			{
				ConfineMouse = m_Window;
			}

			GrabStatus = XGrabPointer( m_pDisplay, m_Window, False,
				EnterWindowMask | LeaveWindowMask | PointerMotionMask,
				GrabModeAsync, GrabModeAsync, ConfineMouse, None,
				CurrentTime );

			XGetPointerControl( m_pDisplay, &m_MouseAccelerationNumerator,
				&m_MouseAccelerationDenominator,
				&m_MouseAccelerationThreshold );

			XChangePointerControl( m_pDisplay, True, True, 1, 1, 0 );

			if( GrabStatus == BadValue )
			{
				zedTrace( "[ZED::System::LinuxWindow::GrabMouse] <ERROR> "
					"Bad value passed to XGrabMouse\n" );

				return ZED_FAIL;
			}

			if( GrabStatus == BadWindow )
			{
				zedTrace( "[ZED::System::LinuxWindow::GrabMouse] <ERROR> "
					"Window is invalid\n" );

				return ZED_FAIL;
			}

			m_MouseGrabbed = ZED_TRUE;

			return ZED_OK;
		}

		void LinuxWindow::ReleaseKeyboard( )
		{
			XUngrabKeyboard( m_pDisplay, CurrentTime );
		}

		void LinuxWindow::ReleaseMouse( )
		{
			XChangePointerControl( m_pDisplay, True, True,
				m_MouseAccelerationNumerator,
				m_MouseAccelerationDenominator,
				m_MouseAccelerationThreshold );

			XUngrabPointer( m_pDisplay, CurrentTime );
			m_MouseGrabbed = ZED_FALSE;
		}

		ZED_SINT32 LinuxWindow::GetXPosition( ) const
		{
			return m_X;
		}

		ZED_SINT32 LinuxWindow::GetYPosition( ) const
		{
			return m_Y;
		}

		ZED_UINT32 LinuxWindow::GetWidth( ) const
		{
			return m_Width;
		}

		ZED_UINT32 LinuxWindow::GetHeight( ) const
		{
			return m_Height;
		}

		void LinuxWindow::SetXPosition( const ZED_SINT32 p_X )
		{
			m_X = p_X;
			XMoveWindow( m_pDisplay, m_Window, m_X, m_Y );
		}

		void LinuxWindow::SetYPosition( const ZED_SINT32 p_Y )
		{
			m_Y = p_Y;
			XMoveWindow( m_pDisplay, m_Window, m_X, m_Y );
		}

		void LinuxWindow::SetWidth( const ZED_UINT32 p_Width )
		{
			if( p_Width > 0 )
			{
				m_Width = p_Width;
				XResizeWindow( m_pDisplay, m_Window, m_Width, m_Height );
			}
		}

		void LinuxWindow::SetHeight( const ZED_UINT32 p_Height )
		{
			if( p_Height > 0 )
			{
				m_Height = p_Height;
				XResizeWindow( m_pDisplay, m_Window, m_Width, m_Height );
			}
		}

		ZED_BOOL LinuxWindow::Resized( )
		{
			if( m_Resized )
			{
				m_Resized = ZED_FALSE;
				return ZED_TRUE;
			}

			return m_Resized;
		}

		ZED_BOOL LinuxWindow::Moved( )
		{
			if( m_Moved )
			{
				m_Moved = ZED_FALSE;
				return ZED_TRUE;
			}

			return m_Moved;
		}

		ZED_UINT32 LinuxWindow::SetXVisualInfo( XVisualInfo *p_pXVisualInfo,
			ZED_BOOL p_Recreate )
		{
			if( p_pXVisualInfo == ZED_NULL )
			{
				zedTrace( "[ZED::System::LinuxWindow::SetXVisualInfo] <ERROR> "
					"XVisualInfo passed in is null\n" );
				return ZED_FAIL;
			}


			if( p_Recreate == ZED_TRUE )
			{
				if( m_Window )
				{
					zedTrace( "[ZED::System::LinuxWindow::SetXVisualInfo] "
						"<INFO> Destoying window at :%d.%d\n", m_DisplayNumber,
						m_ScreenNumber );
					XDestroyWindow( m_pDisplay, m_Window );
				}

				zedSafeDelete( m_pWindowData );
				zedSafeDelete( m_pXVisualInfo );

				m_pXVisualInfo = new XVisualInfo( );
				memcpy( m_pXVisualInfo, p_pXVisualInfo,
					sizeof( XVisualInfo ) );

				this->Create( m_X, m_Y, m_Width, m_Height, m_DisplayNumber,
					m_ScreenNumber, m_Style );
			}
			else
			{
				zedSafeDelete( m_pXVisualInfo );
				m_pXVisualInfo = new XVisualInfo( );
				memcpy( m_pXVisualInfo, p_pXVisualInfo,
					sizeof( XVisualInfo ) );
			}

			return ZED_OK;
		}

		Cursor LinuxWindow::NullCursor( )
		{
			Pixmap CursorMask;
			XGCValues XGC;
			GC GraphicsContext;
			XColor DummyColour;
			Cursor ReturnCursor;

			CursorMask = XCreatePixmap( m_pDisplay, m_Window, 1, 1, 1 );
			XGC.function = GXclear;
			GraphicsContext = XCreateGC( m_pDisplay, CursorMask, GCFunction,
				&XGC );
			XFillRectangle( m_pDisplay, CursorMask, GraphicsContext, 0, 0, 1,
				1 );
			DummyColour.pixel = 0;
			DummyColour.red = 0;
			DummyColour.flags = 4;
			ReturnCursor = XCreatePixmapCursor( m_pDisplay, CursorMask,
				CursorMask, &DummyColour, &DummyColour, 0, 0 );
			XFreePixmap( m_pDisplay, CursorMask );
			XFreeGC( m_pDisplay, GraphicsContext );

			m_CursorHidden = ZED_TRUE;

			return ReturnCursor;
		}
	}
}

