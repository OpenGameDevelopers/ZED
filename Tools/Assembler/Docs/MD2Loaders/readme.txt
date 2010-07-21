------------------------------------------------------------------
|                                                                |
|			Maya Md2 Exporter  (c) Robert Bateman 2005           |
|                                                                |
|                       robthebloke.org                          |
|                                                                |
------------------------------------------------------------------


------------------------------------------------------------------
   	 				 	   Intro
------------------------------------------------------------------

This zip includes just about everything you'd ever want to start
playing with MD2 files. A maya exporter is included (command line)
and a few MD2 loaders ranging from the simple, to the extreme
versions. (sorry about all the #if's in the Md2Load3.exe btw,
If i was writing it again i'd have used templates, wasn't ever
supposed to get quite that silly when i was knocking out the code)


Thanks to, chris sheldon for the yoshi model. Certainly beats
my coder art :|

	www.planetquake.com/polycount/downloads/index.asp?author=153


Thanks also to Ben Humphrey, aka digiben at www.gametutorials.com

  - I started knocking out the exporter using his loader as a test.
Got bored and wrote my own in the end, but the info and links there
are well worth checking out. (His loader code is probably easier
to understand than mine, start there if you're looking for a
tutorial to learn from)



------------------------------------------------------------------
	 				      Licenses
------------------------------------------------------------------

You will need a valid version of Maya 6.0 to run the exporter. The
Md2 file format can only be used for educational purposes as per
the license agreement from ID.

This code is free for you to use for any purpose, as long as you
make no money from it. You are free to re-distribute this code as
long as the entire original zip file remains the same as when you
downloaded it.



------------------------------------------------------------------
	 				     Exporter Usage
------------------------------------------------------------------

I've included a directory with some (really) cack maya binary files
to give you an idea of how to set up your models....
The executables are .exe for Win32, .bin for linux

	Step 1
	------

		Generate a config script for your MD2 file. To do this,
	run either

			Md2Exporter -g
		or
			MD2Exporter -gq2

		The first call generates a rather generic script. You
	probably want this for your own game engines. If you are
	attempting to get the Md2 files into Quake2, then run the
	second version.... [NOTE: I've not actually tried to get the
	md2 files exported into Quake2. If anyone gives it a go, let
	me know if you have any problems].

		A File dialog should open up asking you where you want to
	save the script to. This should go in the same directory as
	your maya binary files!!


	Step 2
	------

		Open the lua script in a text editor and use it to specify
	the skins, meshes and animations. There will be lots of comments
	to help you out, so you might want to read the script's
	comments....


	Step 3
	------

		Run Md2Exporter.exe again but with no options. A file
	Dialog will pop up asking for the lua script. Hopefully that
	should err, just work ;) There should be a directory called
	"textures" with all the output textures as pcx.


	Step 4
	------

		Let me know if the finished game's any-good, I'm always
	interested to see what ppl do with my random code bits.. ;)


------------------------------------------------------------------
	 				     Known Issues
------------------------------------------------------------------

* No glCommands are written to the MD2 file. I couldn't be bothered.
  Its 100% triangles and proud.... Not sure if the numGlCommands
  should have been set to 1 or 0 to indicate none present. I opted
  for zero, but at a guess i reackon that might be wrong :o

* I've not seen any MD2 files with skins contained in them. I assume
  what i'm writing out probably isn't quite up to scratch for Quake2.
  Should be OK for your own game engine though....

* When you init Maya into your own apps, it automatically changes
  the CWD to the maya folder. Basically, if the lua script is not
  in the same directory as the maya files, it will fail. It doesn't
  matter where you put the textures, what you call them, or how big
  you make them.

* The 3rd loader has issues under linux with VBO's. This might be
  my code, or it might be a driver issue. I don't really care,
  i hate linux anyway :p

* The exporter Under Win32 doesn't seem to like the idea of writing
  pcx files. Under Linux it works fine though.... ????

* There may be one or two more problems with this stuff.


------------------------------------------------------------------
				   Re-compiling the code
------------------------------------------------------------------

Under Linux
-----------

in the source directory for the exporter are 2 scripts, mayacc and
mayald. If you need to re-compile with a different version of Maya,
then change the lib and include paths in the scripts and re-build.
It *should* hopefully work (as long as mayacc and mayald are
executable).

Under Win32
-----------

The compiler I'm using at the moment is VC++ 2005 Express Beta1.
You'll need to re-create the solution and project if you're using
anything else. They are all command line apps. Make sure you set
the include and lib paths for the Maya API, and the location of
lua and glew. The other option, is to manually hack the vcproj
files within a text editor. Thats my preffered way of doing it
anyway - not to everyone's liking i'm sure....

Dependencies
------------

The exporter uses lua. The 3rd loader uses glew (unless you turn
off VBO and compressed textures in MD2_Config.h). You might want
to download those libs if you want to re-build the probjects.
glut and openGL are used for all loader examples.

You will also need a valid license for Maya 6.0. The exporter will
not run unless you have that.


------------------------------------------------------------------
------------------------------------------------------------------

