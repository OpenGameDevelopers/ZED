
INCLUDES= -I/usr/X11R6/include -I../../include

# uncomment to compile with debugging enabled
#COMPILE=gcc -g -DDEBUG $(INCLUDES)

# use for final releases
COMPILE=gcc  -DRELEASE $(INCLUDES) -funroll-loops -O3 -ffast-math
LINK=gcc

OBJECTS= main.o pcxLoader.o FrameTimer.o MD2_File.o MD2_Model.o

OUTPUT=../md2Load2.bin

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(LINK) -o $(OUTPUT) $(OBJECTS) -L../../lib -lGL -lGLU -lglut -lGLEW

main.o: main.cpp
	$(COMPILE) -c main.cpp

pcxLoader.o: pcxLoader.c pcxLoader.h
	$(COMPILE) -c pcxLoader.c

FrameTimer.o: FrameTimer.c FrameTimer.h
	$(COMPILE) -c FrameTimer.c

MD2_File.o: MD2_File.cpp MD2_File.h
	$(COMPILE) -c MD2_File.cpp

MD2_Model.o: MD2_Model.cpp MD2_Model.h
	$(COMPILE) -c MD2_Model.cpp

clean:
	rm $(OUTPUT) $(OBJECTS);
