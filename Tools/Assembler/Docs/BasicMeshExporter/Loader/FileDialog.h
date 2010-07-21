//-------------------------------------------------------------------------------
///	\file	FileDialog.h
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

#ifndef __NATIVE_FILE_DIALOGS__H__
#define __NATIVE_FILE_DIALOGS__H__

#ifdef __cplusplus
extern "C" {
#endif

	/*!
	*	\brief	This function creates an open file dialog. This will either use WIN32
	*			Or it will make use of gtk2 if running on linux
	*	\param	Filename	-	this will hold the returned file name & path
	*	\return	1 if OK selected, zero otherwise
	*/
	int OpenFileDialog(char FileName[]);

	/*!
	*	\brief	This function creates an save file dialog. This will either use WIN32
	*			Or it will make use of gtk2 if running on linux
	*	\param	Filename	-	this will hold the returned file name & path
	*	\return	1 if OK selected, zero otherwise
	*/
	int SaveFileDialog(char FileName[]);


#ifdef __cplusplus
}
#endif

#endif
