INCLUDES = -I$(HOME)/cp3/include `pkg-config --cflags gtk+-2.0`
C++      = ./mayacc -g $(INCLUDES)
LINK     = ./mayald -g -L$(HOME)/cp3/lib
CFLAGS   = -O2 -D_BOOL -DLINUX -DUNIX
OUTPUT   = ../Md2Exporter.bin
OBJECTS  = Md2Texture.o Md2Mesh.o Md2Export.o FileDialog.o

all: $(OUTPUT)

# define rule for output app
$(OUTPUT): $(OBJECTS)
	$(LINK) -o $(OUTPUT) $(OBJECTS) -llua -llualib `pkg-config --cflags --libs gtk+-2.0`

FileDialog.o: FileDialog.c FileDialog.h
	$(C++) -c $(CFLAGS) FileDialog.c

Md2Texture.o: Md2Texture.cpp Md2Texture.h
	$(C++) -c $(CFLAGS) Md2Texture.cpp

Md2Export.o: Md2Export.cpp Md2Default.h
	$(C++) -c $(CFLAGS) Md2Export.cpp

Md2Mesh.o: Md2Mesh.cpp Md2Mesh.h
	$(C++) -c $(CFLAGS) Md2Mesh.cpp

clean:
	rm -f $(OBJECTS) $(OUTPUT)
