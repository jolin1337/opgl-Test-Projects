CC=g++
LIBS=-lglut -lGLEW -lGL
utils=./utils/Matrix4.cpp ./utils/Matrix4.h ./utils/Vector.h ./utils/Vector3.h 

all: light cube object
light: ./lights/arc
cube: ./first_cube/arc
object: ./Object_Based/man

./Object_Based/man: Object_Based/main.cpp Object_Based/Data.h Object_Based/setup.h Object_Based/config.h $(utils)
	$(CC) -o ./Object_Based/man Object_Based/main.cpp ./utils/Matrix4.cpp $(LIBS)

./lights/arc: lights/lighting_arc.cpp $(utils)
	$(CC) -o lights/arc lights/lighting_arc.cpp ./utils/Matrix4.cpp $(LIBS)

./first_cube/arc: first_cube/basic_arc.cpp first_cube/setup.h first_cube/config.h $(utils)
	$(CC) -o ./first_cube/arc first_cube/basic_arc.cpp ./utils/Matrix4.cpp $(LIBS)
clean:
	rm -f triangle
