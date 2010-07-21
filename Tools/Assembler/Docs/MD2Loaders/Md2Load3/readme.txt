#######################################################################################################
##
##	Robs Md2 Loader		version 1.0		2-Jan-2005
##
## Thanks to:
##
##		Ben 'digiben' Humphrey, http://www.gametutorials.com  - a fairly useful resource
##
##		chris sheldon for his model of yoshi, the link to planetquake is here.....
##
##    http://www.planetquake.com/polycount/downloads/index.asp?author=153
##
##
##
#######################################################################################################

Right then, there are many tutorials about how to load Md2 files about on the internet. So what makes 
this one different? Well, the answer is that i've tried to show a few bits of useful info about how to
actually incorporate a loader into a game engine and make it efficient enough to handle lots and lots 
of models without crippling your PC ;)


Look in the file MD2_Config.h to play around with some random flags to change how the loader works.
Within main.cpp, you can change the MD2_CREATE_100 flag between 0 and 1 to see if you want to create
100 instances or just the 1. You might also want to play with the Md2Manager::SetStagger() call to
change the update frequency for the models.... setting it at about 3 or 4 seems sensible if you're frame
rate is dropping a bit.



Generally this is a list of things that have bugged me about examples i've seen knocking about on the net:


1) First off, the Quake2 format is very dated. It is basically set up for linear interpolation of vertices.
This is a bit pants, it leads to nasty squishing and flattening of the model quite often. well, theres
not alot i can do about that - it's a product of the animation method. One slightly nasty problem with
vertex animation is that it tends to be very heavy on data storage. The quake2 format actually stores
the keyframed vertices as unsigned chars, and then has a per frame scale and translation key that gets
applied to the key frame to give you the correct vertices.

A number of loaders actually convert this data into floating point values and then store it. This is
pretty inefficient. a float is 4 times the amount of data storage of a byte. So, by doing that you
effectivly increase the memory requirements quite horrifically. It's therefore nicer to store the
data as unsigned chars and LERP the scale and translation keys and apply them at rendering time.



2) The file is loaded in little bits, ie read(verts), read(normals) etc. This is really redundant with a
binary file. Generally it is better to simply read the entire file into memory in one go and use pointer
offsets into the file to extract the data. Once finished with the original data, you have a single buffer
to free....
	With your own custom formats, it's better to save the file as effectively a memory dump that you
can simply walk through to render. Unfortunately since the Md2 is quite old, this isn't really practical
because we want a bit of data conversion in here....



3) Using glBegin / glEnd to render triangles is slow. Display lists really wont help us too much here,
since the data is dynamic. Ideally we want vertex arrays (or VBO's as the new groovy thing). This
requires a bit of data conversion here since vertex arrays and immediate mode data don't get on :(
	We could convert all of the animation keyframes to vertex array fromats and lerp them like that.
It would provide fairly decent performance. I'm not going to do that though, instead I favour using
mappings to go from one to the other. Thus, all animation can be calculated using the original points
and then written into the vertex arrays for a laugh.



4) If we have 2 identical models, the only things that are different between them are the evaluated
vertices and normals; the current animation cycle and time. All other data can be shared. Introduce
an 'instance' class to handle this. If the data always stays the same, put it in the model class. If
the data needs to change to animate the model, put it in the instance.



5) Animations can take a while to calculate, and can therefore eat up a lot of processor cycles. Imo,
animations can be updated at 30fps as a minimum, however we want a higher frame-rate for the game logic.
To save on time, we can simply update one set of instances 1 frame, and the rest on the next frame. By
staggering the updates in this way, we often get much better frame rates. To do this, we need to set up
a management class for the models and their instances.



6) Within your manager class, also provide methods to automatically clean up un-referenced model and
texture data. If two models the same are loaded, dont load all the data twice. Set the manager to
automatically set up refernces. The shared model data should then only be unloaded when all instances
have been deleted. The same should happen for texture data (which tends to be heavier than model data).



7) Get your manager to be self aware, if you have functionality to provide the current data usage, and
frame rates for the model and game updates you can do some fairly cool things. For example, If you are
using too much texture data, delete the highest mip-map level of a fairly un-important texture (ie, one
that hasn't been accessed for a while).
	You can also dynamically change the update staggering dependant on the frame rate vs model update
rate. This way as the load changes due to model visibility, you can keep the anims using minimal CPU
cycles whilst providing the best update rate possible. That way it only looks cack when the load's are
really high.
	Providing memory feedback also helps during development to keep the game's content within fairly
sensible limits. You might be suprised at just how much data your content is using! Always try to keep
it fairly minimal where possible. (at the moment, for a 10,000 vert character with skinning etc, i aim
for about 1Mb of model data for all anim cycles, geometry and texture data. Each instance should then
require about 50Kb)


8) Provide a 'isVisible' flag for each instance. Make sure you update the animation times and instance
movement all the time. The isVisible flag can then be used for rendering and interpolating vertices only.
This should be set externally by some scene management thingy, octree's, bsp, portals, etc. If your scene
management is fairly efficient, you'll be having hundreds on enemies to kill (as long as you dont have them
all on screen at once).


9) If possible, split the vertex array data and upload any static data as static onto the graphics card.
For example, tex coords and face indices rarely change. If you use VBO, it's fairly easy to do this,
the gain is a big time saving since most of the meshes data will be in Vram, not main system memory.


10) provide a mechanism to change the materials/textures on a model. You'll be suprised how different you
can make things look. Unfortunately, the Md2 file isn't skinned. If it used bone animations, you can also
support different geometry on the same base skeleton. This saves on a lot of data storage!!!
	The md2 file does however handle multiple texture names within the file. Always handy for making things
look a wee bit different.



11) Don't believe everything in the Nvidia and ATI samples!! Just because you can do many things in hardware,
doesn't mean that you want to. In the case of an Md2 file, loading 2 keyframes into VBO's and using
vertex blending extensions or vertex programs is a win. Since it may take 0.2 secs between the 2 keys, uploading
the data into VBO's 5 times a second is definately going to be fast. Using the vector maths hardware to
LERP them is going to be very quick since you only need to change the constant lerp factor as the anim updates.
You should be able to get every model updating at a realtime rate quite *failry* easily.

Things that are positivly a bad idea to put into hardware are :

		Parametric Surface calculation
		------------------------------
			GPU's can't compete with lazy evaluation and buffering

		Skinning
		--------
			requires that you duplicate calculation due to vertex array data being larger than the original
		points. Always do it in software and expand into vertex arrays, it's always faster!

		BlendShapes
		-----------
			This requires you to store copies of the entire mesh in vertex array format and then accumulate
		them in hardware. Always a bad idea. If a blend shape only distorts the points around say an eyebrow,
		why not just store the offsets of the vertices that move and the actual indices of the vertices.
		Accumulating these in software will be *MUCH* better than using vertex programs (and save on loads of
		memory usage).



err, well thats turned into a general rant for no reason. never mind.... ;)

enjoy!

