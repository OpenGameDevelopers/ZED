//---------------------------------------------------------------------------------------
/// \file	main.cpp
/// \author	Rob Bateman
/// \date	1-1-2005
/// \brief	This is a 'simple' example of how to load and handle Md2 files. 
///
///
//---------------------------------------------------------------------------------------


#include <stdlib.h>
#include <iostream>

// GL includes
#include "MD2_Model.h"
#include "pcxLoader.h"
#include <GL/glut.h>
#include "FrameTimer.h"

// user instructions
const char* g_InfoText = "Keys 0 to 9 set anims, 0 to 9            \n"
						 "Keys q to p set anims, 10 to 19          \n"
						 "The mouse moves about the scene (aka, maya controls)\n\n";


/// a pointer to the model instance created
MD2Model md2File;
unsigned int md2Texture;

/// camera variables for maya(ish) style mouse movement
struct {
	float zoom;
	float rotx;
	float roty;
	float tx;
	float ty;
	int lastx;
	int lasty;
	bool Buttons[3];
}
Mouse = {
	100.0f,	0.01f,	0.01f,	0.01f,	0.01f,	0,  0,	{false,false,false}
};

/// A function to load a bitmap file and return the texture object for that texture
unsigned int MakeTexture(const char* filename) {

	unsigned int w,h,bpp;
	unsigned char* pixels;

	if(!LoadPcxImage(filename,&pixels,&w,&h,&bpp)) {
		return 0;
	}

	GLenum infmt,outfmt;
	switch(bpp) {
	case 3:
		infmt = GL_RGB;
		outfmt = GL_RGB;
		break;
	case 4:
		infmt = GL_RGBA;
		outfmt = GL_RGBA;
		break;
	case 1:
		infmt = outfmt = GL_ALPHA;
		break;
	case 2:
		infmt = outfmt = GL_RGB5_A1;
		break;
	default:
		free(pixels);
		return 0;
	}
	
	unsigned int tex_obj=0;
	glGenTextures(1,&tex_obj);

	glBindTexture (GL_TEXTURE_2D, tex_obj);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0,outfmt,w,h,0,infmt,GL_UNSIGNED_BYTE,pixels);

	return tex_obj;
}

/// do custom app initialisation here
void OnInit() {

	// write some useful info
	std::cout << g_InfoText;

	// load the Md2 file 
	md2File.Load("tris.md2");

	// write memory usage
	std::cout << "memory usage " << (md2File.GetDataSize()/1024.0f) << "kb\n";

	md2Texture = MakeTexture("yoshi.pcx");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	// let the frame timer commence...
	InitFrameTimer();

	glColor3f(1,1,1);
}

/// delete any resources or dynamic data here....
void OnExit() {
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

	// apply texture
	glBindTexture(GL_TEXTURE_2D,md2Texture);

	// render all model instances loaded
	md2File.Render();

	glutSwapBuffers();
}

// 
void OnKeyPress(unsigned char key,int x,int y) {

	switch(key) {

		case '0':	md2File.SetAnim(0);		break;
		case '1':	md2File.SetAnim(1);		break;
		case '2':	md2File.SetAnim(2);		break;
		case '3':	md2File.SetAnim(3);		break;
		case '4':	md2File.SetAnim(4);		break;
		case '5':	md2File.SetAnim(5);		break;
		case '6':	md2File.SetAnim(6);		break;
		case '7':	md2File.SetAnim(7);		break;
		case '8':	md2File.SetAnim(8);		break;
		case '9':	md2File.SetAnim(9);		break;

		case 'q':	md2File.SetAnim(10);	break;
		case 'w':	md2File.SetAnim(11);	break;
		case 'e':	md2File.SetAnim(12);	break;
		case 'r':	md2File.SetAnim(13);	break;
		case 't':	md2File.SetAnim(14);	break;
		case 'y':	md2File.SetAnim(15);	break;
		case 'u':	md2File.SetAnim(16);	break;
		case 'i':	md2File.SetAnim(17);	break;
		case 'o':	md2File.SetAnim(18);	break;
		case 'p':	md2File.SetAnim(19);	break;

		default: break;
	}
}

/// used as a hacky flag to spit out the frame rate every 100 frames
int g_FrameRateCounter=0;

/// the update loop of the program
void OnIdle() {

	SortFrameTimer();

	// get time since last frame
	float dt = FrameTime();

	//
	// update all instances animation
	//
	md2File.Update(dt);

	// only output the frame rate every 100 frames...
	if(++g_FrameRateCounter>500) {
		std::cout << (1.0f/dt) << " fps\r";
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
	glutCreateWindow("Simple MD2 Loader Example");

	// register callback functions
	glutDisplayFunc(OnDraw);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKeyPress);
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
