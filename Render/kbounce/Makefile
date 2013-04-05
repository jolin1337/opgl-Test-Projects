CC=g++
LIBS=-lglut -lGLEW -lGL
utils=utils/Matrix4.h utils/Vector3.h utils/Vector.h
utils_cpp=utils/Matrix4.cpp

all: kbounce
	./kbounce

kbounce: main.cpp Data.h setup.h config.h Character.h $(utils) $(utils_cpp)
	$(CC) -o kbounce main.cpp $(utils_cpp) $(LIBS)