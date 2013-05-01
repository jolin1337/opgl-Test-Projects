CC=g++
LIBS=-lglut -lGLU -lGLEW -lGL
utils=utils/Matrix4.h utils/Vector3.h utils/Vector.h
utils_cpp=utils/Matrix4.cpp

CPPS    = World.cpp               BlockSet/Block/Block.cpp BlockSet/BlockSet.cpp
HEADERS = World.h config.h Data.h BlockSet/Block/Block.h   BlockSet/BlockSet.h 

OUTPUT=game

all: build

build: $(OUTPUT)

$(OUTPUT): main.cpp $(HEADERS) $(CPPS) $(utils) $(utils_cpp)
	$(CC) -o $(OUTPUT) main.cpp $(CPPS) $(utils_cpp) $(LIBS)

run: build
	./$(OUTPUT)

clean:
	rm -f $(OUTPUT)

cleanbuild: clean build