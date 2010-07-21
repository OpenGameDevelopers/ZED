C++		= g++
CFLAGS		= -D_BOOL -mcpu=pentiumpro -fno-gnu-keywords -fno-for-scope -DLINUX -DUNIX
INCLUDES	= -I. -I/usr/aw/maya5.0/include  `pkg-config --cflags gtk+-2.0`
LIBS		= `pkg-config --cflags --libs gtk+-2.0`
LD		= ./mayald
COMPILE 	= gcc -c $(CFLAGS) $(INCLUDES)
OUTPUT=../mayaexporter

all: $(OUTPUT)

$(OUTPUT): main.o Mesh.o FileDialog.o
	$(LD) -o $(OUTPUT) main.o Mesh.o FileDialog.o $(LIBS)
	
main.o: main.cpp
	$(COMPILE) main.cpp 
	
Mesh.o: Mesh.cpp Mesh.h
	$(COMPILE) Mesh.cpp 

FileDialog.o: FileDialog.c FileDialog.h
	$(COMPILE) FileDialog.c 
clean:
	rm -f *.o

