
INCLUDES= -I/usr/X11R6/include -I../../include

# uncomment to compile with debugging enabled
#COMPILE=gcc -g -DDEBUG $(INCLUDES)

# use for final releases
COMPILE=gcc  -DRELEASE $(INCLUDES) -funroll-loops -O3 -ffast-math
LINK=gcc

OBJECTS= main.o bmpLoader.o FrameTimer.o MD2_File.o MD2_Model.o tgaLoader.o pcxLoader.o MD2_Manager.o

OUTPUT=../md2Load3.bin

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(LINK) -o $(OUTPUT) $(OBJECTS) -L../../lib -lGL -lGLU -lglut -lGLEW

main.o: main.cpp MD2_Config.h
	$(COMPILE) -c main.cpp

bmpLoader.o: bmpLoader.c bmpLoader.h
	$(COMPILE) -c bmpLoader.c

tgaLoader.o: tgaLoader.c tgaLoader.h
	$(COMPILE) -c tgaLoader.c

pcxLoader.o: pcxLoader.c pcxLoader.h
	$(COMPILE) -c pcxLoader.c

FrameTimer.o: FrameTimer.c FrameTimer.h
	$(COMPILE) -c FrameTimer.c

MD2_File.o: MD2_File.cpp MD2_File.h MD2_Config.h
	$(COMPILE) -c MD2_File.cpp

MD2_Model.o: MD2_Model.cpp MD2_Model.h MD2_Config.h
	$(COMPILE) -c MD2_Model.cpp

MD2_Manager.o: MD2_Manager.cpp MD2_Manager.h MD2_Config.h
	$(COMPILE) -c MD2_Manager.cpp

clean:
	rm $(OUTPUT) $(OBJECTS);
