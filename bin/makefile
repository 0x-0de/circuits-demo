CC = g++
CPPFLAGS = -std=c++11 -Wall -g -c

LIBDIR = -L./
LIBS = -lpthread -lopengl32 -lgdi32 -lglfw3 -lglew32

OBJLIST = main.o utilmath.o mesh.o renderer.o shader.o canvas.o node.o queue.o

default: $(OBJLIST)
	$(CC) $(OBJLIST) $(LIBDIR) $(LIBS) -o Circuitbuilder.exe
	
main.o: ../src/main.cpp
	$(CC) $(CPPFLAGS) ../src/main.cpp

utilmath.o: ../src/utils/utilmath.h ../src/utils/utilmath.cpp
	$(CC) $(CPPFLAGS) ../src/utils/utilmath.cpp

mesh.o: ../src/rendering/mesh.h ../src/rendering/mesh.cpp
	$(CC) $(CPPFLAGS) ../src/rendering/mesh.cpp

renderer.o: ../src/rendering/renderer.h ../src/rendering/renderer.cpp
	$(CC) $(CPPFLAGS) ../src/rendering/renderer.cpp

shader.o: ../src/rendering/shader.h ../src/rendering/shader.cpp
	$(CC) $(CPPFLAGS) ../src/rendering/shader.cpp

canvas.o: ../src/model/canvas.h ../src/model/canvas.cpp
	$(CC) $(CPPFLAGS) ../src/model/canvas.cpp

node.o: ../src/model/node.h ../src/model/node.cpp
	$(CC) $(CPPFLAGS) ../src/model/node.cpp

queue.o: ../src/model/queue.h ../src/model/queue.cpp
	$(CC) $(CPPFLAGS) ../src/model/queue.cpp

clean:
	-rm Circuitbuilder.exe *.o