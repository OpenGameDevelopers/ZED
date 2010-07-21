
INCLUDES=-I/usr/X11R6/include `pkg-config --cflags gtk+-2.0`
LIB=-L/usr/X11R6/lib `pkg-config --cflags --libs gtk+-2.0`

#COMPILE=gcc -g -DDEBUG $(INCLUDES)
COMPILE=gcc -O2 -DRELEASE $(INCLUDES)

OBJECTS=main.o Mesh.o FileDialog.o
OUTPUT=../mayaloader

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(COMPILE) -o $(OUTPUT) $(OBJECTS) $(LIB) -lGL -lGLU -lglut -lstdc++ -lXmu -lXi 
	
main.o: main.cpp
	$(COMPILE) -c main.cpp
	
Mesh.o: Mesh.cpp Mesh.h
	$(COMPILE) -c Mesh.cpp
	
FileDialog.o: FileDialog.c FileDialog.h
	$(COMPILE) -c FileDialog.c

clean:
	rm loader $(OBJECTS);
