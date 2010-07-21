//---------------------------------------------------------------------------------------
/// \file	main.cpp
/// \author	Rob Bateman
/// \date	1-1-2005
/// \brief	This is a wee example of how to use my md2 loader. It's a little bit more 
///			specced up than most md2 loader examples out there, it may not be the best 
///			thing to learn from, i'd have a look at www.gametutorials.com for their 
///			tutorial on the md2 loader - this one is a lot more advanced. Ideally you
///			should know a little bit about loading the format before trying to get your
///			head around my quirks.
///
///			Basically, this loader is set up in a more realistic fashion for what you 
///			want within a game engine. The Md2Manager class impliments a number of 
///			things to provide a global level of control over the model data. It maintains
///			all models - you will not be able to access the data!! 
///
///			Basically, the textures and models are reference counted. ie, if you load the
///			same model more than once, the actual model data will be shared and 2 instances
///			will be created. Simply put, the Md2Instance holds everything that changes as a
///			model animates: ie, the vertices and normals, the position in the current anim 
///			cycle etc. The Md2Model class holds all data that does not change, ie keyframes,
///			triangle indices, texture coords.... 
///
///			If you delete a model instance, and there are no more instances for the specified
///			model, the data will automatically be released. The same thing happens with texture
///			data. 
///
///			In addition, there are a number of other funky features to allow you to stagger
///			updates of instances. This should help in situations where your app is bandwidth
///			limited. Most of these things wont help too much until your on screen character
///			counts start exceeding 100 or so.
///
///			The best reference is to have a look at the Md2Instance class within MD2_Model.h
///			- this should explain most of what you need to know. The Md2Manager class should
///			also provide a good indication of the global controls available to you.
///
///			There are also a few flags defined within MD2_Config.h that may help you to
///			customise some performance and qualiity settings for you.
///
///	\note	I use the glew lib to initialise all openGL extensions (because i'm lazy). 
///			This code will compile with openGl 1.1 if you set MD2_USE_VBO and 
///			MD2_USE_COMPRESSED_TEXTURES to zero at the top of MD2_Config.h. 
///
//---------------------------------------------------------------------------------------


//	Set this to 1 to test the program with 100 instances rather than 1
//
#define MD2_CREATE_100 1





#include <stdlib.h>
#include <iostream>

// GL includes
#include "MD2_Manager.h"
#include <GL/glut.h>
#include "FrameTimer.h"

// link to glew for openGL extensions (only if needed)
#ifdef WIN32
	#if (MD2_USE_VBO || MD2_USE_COMPRESSED_TEXTURES)
		#pragma comment(lib,"glew32.lib")
	#endif
#endif

// my image loaders. Not the greatest pieces of code but oh well..
#include "bmpLoader.h"
#include "tgaLoader.h"
#include "pcxLoader.h"

const char* g_InfoText = "Cursor Keys control character movement   \n"
						 "Keys 0 to 9 set anims, 0 to 9            \n"
						 "Keys q to p set anims, 10 to 19          \n"
						 "The mouse moves about the scene (aka, maya controls)\n\n";


/// a pointer to the model instance created
Md2Instance* myInstance = 0;

/// display list for grid
unsigned int displayList=0;

// camera variables for maya(ish) style mouse movement
struct {
	float zoom;
	float rotx;
	float roty;
	float tx;
	float ty;
	int lastx;
	int lasty;
	// mouse button states
	bool Buttons[3];
}
Mouse = {
	100.0f,	0.01f,	0.01f,	0.01f,	0.01f,	0,  0,	{false,false,false}
};




/// do custom app initialisation here
void OnInit() {

	// print controls
	std::cout << g_InfoText;

	glClearColor(0.4f,0.4f,0.45f,0.0f);

	// initialise openGL extensions
	#if (MD2_USE_VBO || MD2_USE_COMPRESSED_TEXTURES)
		glewInit();
	#endif

	// register loaders for supported texture file formats
	Md2Manager::RegisterTexLoader("bmp",LoadBmpImage);
	Md2Manager::RegisterTexLoader("pcx",LoadPcxImage);
	Md2Manager::RegisterTexLoader("tga",LoadTgaImage);

	// this sets an update stagger for the animation updates. Basically, if set to 1, each
	// instance is updated every frame. If 2, then the updates are staggered so that the
	// updates occur every other frame. This reduces the work done and amount of time that 
	// the VBO data gets updated.
	//
	Md2Manager::SetStagger(4);

	// can toggle between a single test instance and 100
	//
	#if !MD2_CREATE_100
	
		// load the Md2 file via the manager. This takes the name of the file and the frame-rate
		// of the animation cycles.
		//
		// It returns a new instance
		//
		myInstance = Md2Manager::Load("tris.md2",5);

		// make sure it's ok...
		if( !myInstance ) {	
			std::cerr << "Unable to load the md2 file!" << std::endl;
			exit(0);
		}

		// set a texture for the instance. It's up to you if you specify an extension or not.
		// if no extension is specified, then the code will simply try appending each supported
		// extension to the filename to see if that file exists. ie, if you converted hobgoblin.bmp
		// to hobgoblic.pcx, the code would still load the file.
		//
		// If the file is specified with a full path, it will try the full path first, if that fails,
		// then it will strip the path to try it in the current working directory. If that fails it
		// will take the path of the original model file and see if the texture exists in there.
		//
		// If the image is of funny dimensions, ie 260x260 instead of 256x256, it will resize the image
		// data. This might not work all the time, it can go a bit funny  :|
		//
		myInstance->SetTexture("yoshi");

		// don't set this to 1, it would be silly.... 
		#if 0
			// In this case we specify a texture manually, this is because this particular file has
			// no skins listed within it. To use a skin listed in the file (my exporter handles them),
			// you should first query the number of skins, like so....
			//
			unsigned int numSkins = myInstance->GetNumSkins();
			for( unsigned int i=0;i!=numSkins;++i )
			{
				std::cout << "Skin Name: \"" << myInstance->GetSkinName(i) << "\"\n";
			}

			// then specify the skin ID in this manner...
			// FYI: this is a shorthand way of writing .... 
			//
			//		myInstance->SetTexture( myInstance->GetSkinName(0) ) 
			//
			myInstance->SetSkin(0);

		#endif
			
	#else
	
		//
		// Some quick results for my laptop (nvidia geforce go FX - pretty pants :( )
		// using unsigned chars for anim data, VBO's for rendering, no BBox or BSphere calculation
		//
		// With normal calculation
		//		110 fps    models at 26 fps,  2.20Mb data
		//
		// Without normal calculation
		//		235 fps	   models at 56 fps,  1.27Mb data
		//
		// Setting different stagger amounts affects the frame rate quite a bit ;)
		//

		// loop through to create 100 model instances
		for( int i = -225; i <250; i += 50 )
		{
			for( int j = -225; j < 250; j += 50 )
			{
				// load model
				myInstance = Md2Manager::Load("tris.md2",5);

				// make sure it's ok...
				if( !myInstance ) {
					std::cerr << "Unable to load the md2 file!" << std::endl;
					exit(0);
				}
				// assign texture
				myInstance->SetTexture("yoshi");

				// use a random cycle
				myInstance->SetCycle( rand() % 20 );

				// set it's position
				myInstance->SetPosition(i,0,j);
			}
		}
	#endif


	unsigned int numSkins = myInstance->GetNumSkins();
	for( unsigned int i=0;i!=numSkins;++i )
	{
		std::cout << "Skin" << i << " Name: \"" << myInstance->GetSkinName(i) << "\"\n";
	}


	// dump some info about how much memory is used for instances, global buffers, textures
	// and models. Gives a fairly useful breakdown of what data is allocated where.
	//
	std::cout << "mem usage - model VBO's    = " << (Md2Manager::GetMemoryUsage(MEM_MODEL_VBO)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - instance VBO's = " << (Md2Manager::GetMemoryUsage(MEM_INSTANCE_VBO)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - model data     = " << (Md2Manager::GetMemoryUsage(MEM_MODEL)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - instance data  = " << (Md2Manager::GetMemoryUsage(MEM_INSTANCE)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - global buffer  = " << (Md2Manager::GetMemoryUsage(MEM_GLOBAL)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - textures       = " << (Md2Manager::GetMemoryUsage(MEM_TEXTURES)/1024.0f) << "kb" << std::endl;
	std::cout << "mem usage - total          = " << (Md2Manager::GetMemoryUsage(MEM_ALL)/1024.0f) << "kb" << std::endl;

	Md2Manager::Update(0.01f);

	glEnable(GL_DEPTH_TEST);
//	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glEnable(GL_CULL_FACE);
	#if MD2_USE_NORMALS
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
	#endif

	// make display list to draw the floor
	displayList = glGenLists(1);
	glNewList(displayList,GL_COMPILE);
		glDisable(GL_TEXTURE_2D);
		#if MD2_USE_NORMALS
			glDisable(GL_LIGHTING);
		#endif
		glColor3f(0.2f,0.2f,0.2f);
		glBegin(GL_LINES);
		for(int i=-250;i<=250;i+=25) {
			glVertex3f(i,-25.0f,250.0f);
			glVertex3f(i,-25.0f,-250.0f);
			glVertex3f(250.0f,-25.0f,i);
			glVertex3f(-250.0f,-25.0f,i);
		}
		glEnd();
		glColor3f(1.0f,1.0f,1.0f);
		#if MD2_USE_NORMALS
			glEnable(GL_LIGHTING);
		#endif
		glEnable(GL_TEXTURE_2D);
	glEndList();

	// let the frame timer commence...
	InitFrameTimer();

	glColor3f(1,1,1);
}

/// delete any resources or dynamic data here....
void OnExit() {
	Md2Manager::Release();
}

/// used to render the contents of the window
void OnDraw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	// set camera position
	glTranslatef(0,0,-Mouse.zoom);
	glTranslatef(Mouse.tx,Mouse.ty,0);
	glRotatef(Mouse.rotx,1,0,0);
	glRotatef(Mouse.roty,0,1,0);

	glCallList(displayList);

	// render all model instances loaded
	Md2Manager::Render();

	glutSwapBuffers();
}



/// here i use myInstance->SwitchCycle( animId, time ); to switch the currently playing anim.
/// This is not exactly brilliant, but basically it blends from the current anim to the new anim
/// over the specified amount of seconds. In this case i'm simply switching to the animations
/// using a 0.5 second blend time.
///
///	Ideally you want a state machine to add a slightly higher level of control here. I would
/// suggest using lua or another scripting language to set this up, it makes it easy to change 
/// without re-compiling....
///
void OnKeyPress(unsigned char key,int x,int y) {

	switch(key) {

		case '0':	myInstance->SwitchCycle(0,0.5f,false);	break;
		case '1':	myInstance->SwitchCycle(1,0.3f,false);	break;
		case '2':	myInstance->SwitchCycle(2,0.5f);		break;
		case '3':	myInstance->SwitchCycle(3,0.5f);		break;
		case '4':	myInstance->SwitchCycle(4,0.5f);		break;
		case '5':	myInstance->SwitchCycle(5,0.5f);		break;
		case '6':	myInstance->SwitchCycle(6,0.2f);		break;
		case '7':	myInstance->SwitchCycle(7,0.5f);		break;
		case '8':	myInstance->SwitchCycle(8,0.5f);		break;
		case '9':	myInstance->SwitchCycle(9,0.5f);		break;

		case 'q':	myInstance->SwitchCycle(10,0.5f);		break;
		case 'w':	myInstance->SwitchCycle(11,0.5f);		break;
		case 'e':	myInstance->SwitchCycle(12,0.5f);		break;
		case 'r':	myInstance->SwitchCycle(13,0.3f,false);	break;
		case 't':	myInstance->SwitchCycle(14,0.5f);		break;
		case 'y':	myInstance->SwitchCycle(15,0.5f);		break;
		case 'u':	myInstance->SwitchCycle(16,0.5f);		break;
		case 'i':	myInstance->SwitchCycle(17,0.5f);		break;
		case 'o':	myInstance->SwitchCycle(18,0.5f);		break;
		case 'p':	myInstance->SwitchCycle(19,0.5f);		break;

		default: break;
	}
}




bool g_KeyStates[128] = {0};

/// handles a key press of the special keys
void OnSpecialKeyPress(int key,int x,int y) {
	g_KeyStates[key]=true;
}

/// handles a key release of the special keys
void OnSpecialKeyRelease(int key,int x,int y) {
	g_KeyStates[key]=false;
}

/// used as a hacky flag to spit out the frame rate every 100 frames
float g_FrameRateCounter=0;

/// the update loop of the program
void OnIdle() {

	SortFrameTimer();

	// get time since last frame
	float dt = FrameTime();

	//
	// update all instances animation
	//
	Md2Manager::Update(dt);

	//
	// relative character movement
	//
	if(g_KeyStates[GLUT_KEY_DOWN]) {
		myInstance->Move(-dt*100,0,0,true);
	}
	if(g_KeyStates[GLUT_KEY_UP]) {
		myInstance->Move(dt*100,0,0,true);
	}

	//
	// relative character rotation
	//
	if(g_KeyStates[GLUT_KEY_LEFT]) {
		myInstance->Rotate(dt*360);
	}
	if(g_KeyStates[GLUT_KEY_RIGHT]) {
		myInstance->Rotate(-dt*360);
	}

	// only output the frame rate every 100 frames...
	if((g_FrameRateCounter+=dt)>1) {
		std::cout << (1.0f/dt) << " fps  model fps = " << Md2Manager::CurrentModelFps()
		#ifdef WIN32
			<< "\r";
		#else
			<< "\n";
		#endif
		g_FrameRateCounter=0;
	}

	glutPostRedisplay();
}



/// used to handle changes in screen resolution
void OnResize(int w,int h) {
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,5,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/// used to handle mouse button presses and releases
void OnMouseButton(int button, int state,int x,int y) {
	bool Pressed = (state==GLUT_DOWN)? true : false;

	Mouse.lastx=x;
	Mouse.lasty=y;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		Mouse.Buttons[0] = Pressed;
		break;
	case GLUT_MIDDLE_BUTTON:
		Mouse.Buttons[1] = Pressed;
		break;
	case GLUT_RIGHT_BUTTON:
		Mouse.Buttons[2] = Pressed;
		break;
	default:
		break;
	}
}

/// handles the movement of the mouse when a button is held
void OnMouseMove(int x,int y) {
	int diffx=x-Mouse.lastx;
	int diffy=y-Mouse.lasty;
	Mouse.lastx=x;
	Mouse.lasty=y;

	if( Mouse.Buttons[0] && (Mouse.Buttons[1]||Mouse.Buttons[2]) )
	{
		Mouse.zoom -= (float) 0.5f * diffx;
	}
	else
		if( Mouse.Buttons[0] )
		{
			Mouse.rotx += (float) 0.5f * diffy;
			Mouse.roty += (float) 0.5f * diffx;
		}
		else
			if( (Mouse.Buttons[1]||Mouse.Buttons[2]) )
			{
				Mouse.tx += (float) 0.5f * diffx;
				Mouse.ty -= (float) 0.5f * diffy;
			}
}

/// the main entry point of the app....
int main(int argc,char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	glutCreateWindow("MD2 Loader Example");

	// register callback functions
	glutDisplayFunc(OnDraw);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKeyPress);
	glutSpecialFunc(OnSpecialKeyPress);
	glutSpecialUpFunc(OnSpecialKeyRelease);
	glutMouseFunc(OnMouseButton);
	glutMotionFunc(OnMouseMove);
	glutIdleFunc(OnIdle);

	// register the exit function
	atexit(OnExit);

	// call custom initialisation
	OnInit();

	// enter the main loop for the application
	glutMainLoop();

	return 0;
}
