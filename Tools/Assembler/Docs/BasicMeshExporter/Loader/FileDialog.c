//-------------------------------------------------------------------------------
///	\file	FileDialog.cpp
///	\author	Robert	Bateman
/// \brief	This just provides some linux and win32 file dialogs. Under Linux it uses gtk2
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

#ifndef WIN32
#include <gtk/gtk.h>
#include <string.h>
#include <assert.h>

extern int   g_argc;
extern char** g_argv;

char g_FileName[512];

//-------------------------------------------------------------------------------
/* Get the selected filename and print it to the console */
static void file_ok_sel( GtkWidget        *w,
                         GtkFileSelection *fs )
{
	strcpy(g_FileName,gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}

//-------------------------------------------------------------------------------
int OpenFileDialog(char FileName[])
{
    GtkWidget *filew;

	g_FileName[0] = '\0';

    gtk_init (&g_argc, &g_argv);

    /* Create a new file selection widget */
    filew = gtk_file_selection_new ("File selection");

    g_signal_connect (G_OBJECT (filew), "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);

    /* Connect the ok_button to file_ok_sel function */
    g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", G_CALLBACK (file_ok_sel), (gpointer) filew);

    /* Connect the cancel_button to destroy the widget */
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
							 "clicked",
							 G_CALLBACK (gtk_widget_destroy),
			      			 G_OBJECT (filew));
    /* Connect the cancel_button to destroy the widget */
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
							 "clicked",
							 G_CALLBACK (gtk_widget_destroy),
			      			 G_OBJECT (filew));

    gtk_widget_show (filew);
    gtk_main ();

	if(g_FileName[0] != '\0') {
		strcpy(FileName,g_FileName);
		return 1;
	}

    return 0;
}

//-------------------------------------------------------------------------------
int SaveFileDialog(char FileName[]) {
	return OpenFileDialog(FileName);
}


#else
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//-------------------------------------------------------------------------------
int OpenFileDialog(char FileName[])
{
	char Dir[512];
	char fn[512] = {0};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = fn;
	ofn.nMaxFile = 512;
	ofn.lpstrTitle = "Open File";


	GetCurrentDirectory(512,Dir);
	if( GetOpenFileName(&ofn) )
	{
		int i;
		for (i=0;i<512;++i)
		{
			if( fn[i] == '\\' )
				fn[i] = '/';
		}
		strcpy(FileName,fn);
		SetCurrentDirectory(Dir);
		return 1;
	}
	FileName[0] = '\0';
	SetCurrentDirectory(Dir);
	return 0;
}

//-------------------------------------------------------------------------------
int SaveFileDialog(char FileName[])
{
	char Dir[512];
	char fn[512] = {0};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = fn;
	ofn.nMaxFile = 512;
	ofn.lpstrTitle = "Save File";
	GetCurrentDirectory(512,Dir);
	if( GetSaveFileName(&ofn) )
	{
		int i;
		for (i=0;i<512;++i)
		{
			if( fn[i] == '\\' )
				fn[i] = '/';
		}
		strcpy(FileName,fn);
		SetCurrentDirectory(Dir);
		return 1;
	}
	FileName[0] = '\0';
	SetCurrentDirectory(Dir);
	return 0;
}
#endif


















