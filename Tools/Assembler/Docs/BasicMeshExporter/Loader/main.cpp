///-----------------------------------------------------------------------------
/// 
/// \file	main.cpp
/// \author	Rob Bateman
/// \brief	A simple viewer for a simple file format.
/// \note
///
/// 		This sourcefile is provided as-is and no guarentee or warrenty
///			is provided for any damage that may arise from using it. This
///			code is free for your own use, the only thing i ask is a small
///			credit somewhere for my work. An e-mail saying you found it
///			useful would also be much appreciated by myself ;)
///
///			If you find any bugs arising to maya version changes or you
///			have improvements to the source code, then feel free to mail
///			them to me and i'll update the source-code on the website.
///			Before you e-mail bug reports, make sure that it is my code
///			causing the problem, and not some wierd oddity in the maya
///			scene. This code is designed to handle most common model set-ups,
///			it is not designed to replicate perfectly everything that maya
///			does.
///
///			DO NOT send me feature requests, i'm not likely to add to this
///			code myself unless something really new and groovy comes along.
///			This is mainly due to my current time restrictions. If you do
///			have code written that adds something fairly groovy to the
///			exporter, then i will happily update the exporter code and give
///			full credit where due.....
///
///			Robert Bateman
///
///				email code to:    rbateman@bmth.ac.uk
///				contact:          robthebloke@hotmail.com
///
//-------------------------------------------------------------------------------

#include <iostream>
#include <GL/glut.h>

#include "Mesh.h"
#include "FileDialog.h"

/// a variable used to rotate the object
float rot = 0;

/// a list of all the meshes loaded
std::vector<XMesh*> meshes;

//-------------------------------------------------------------------------------
/// \brief	Initialises the openGL scene
/// 
void Init() 
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------------------------
/// \brief	Draws the meshes 
/// 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	gluLookAt(2,8,20,0,0,0,0,1,0);

	glPushMatrix();

		glRotatef(rot,0,1,0);

		/// draw all of the meshes loaded from the file
		DrawMeshes(meshes);

	glPopMatrix();

	glutSwapBuffers();
}

//-------------------------------------------------------------------------------
/// \brief	Called when the screen gets resized
/// \param	w	-	the new width
/// \param	h	-	the new height
/// 
void reshape(int w, int h)
{
	// prevent divide by 0 error when minimised
	if(w==0) 
		h = 1;

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)w/h,0.1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-------------------------------------------------------------------------------
/// \brief	Called when nothing else to do, simply rotates the scene
/// 
void idle()
{
	rot+=0.05f;
	glutPostRedisplay();
}


//-------------------------------------------------------------------------------
/// entry point. A little bit of a hack is required for linux file dialogs using gtk
/// 
#ifdef WIN32
int main(int argc,char** argv)
{

#else
int g_argc=0;
char** g_argv=0;
int main(int argc,char** argv)
{
	g_argc = argc;
	g_argv = argv;

#endif

	// if filename specified as an arg
	if( argc == 2 )
		// load mesh file
		LoadMeshFile(argv[1],meshes);
	else
	{
		// if no file specified, use a file dialog to get the name
		char buffer[512] = {0};
		if( !OpenFileDialog(buffer) ) {
			return 0;
		}
		LoadMeshFile(buffer,meshes);
	}
	
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,100);
	glutCreateWindow("hello");


	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	Init();

	glutMainLoop();
}

























