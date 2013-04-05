CC=g++
LIBS=-lglut -lGLEW -lGL
utils=utils/Matrix4.h utils/Vector3.h utils/Vector.h
utils_cpp=utils/Matrix4.cpp

all: game object light cube
	./g_run
game: ./first_game/man g_run
object: ./Object_Based/man o_run
light: ./lights/arc l_run
cube: ./first_cube/arc c_run

./first_game/man: first_game/main.cpp first_game/Data.h first_game/setup.h first_game/config.h first_game/Character.h $(utils) $(utils_cpp)
	$(CC) -o ./first_game/man first_game/main.cpp $(utils_cpp) $(LIBS)

./Object_Based/man: Object_Based/main.cpp Object_Based/Data.h Object_Based/setup.h Object_Based/config.h $(utils) $(utils_cpp)
	$(CC) -o ./Object_Based/man Object_Based/main.cpp $(utils_cpp) $(LIBS)


./lights/arc: lights/lighting_arc.cpp $(utils) $(utils_cpp)
	$(CC) -o ./lights/arc lights/lighting_arc.cpp $(utils_cpp) $(LIBS)

./first_cube/arc: first_cube/basic_arc.cpp first_game/setup.h first_game/config.h $(utils) $(utils_cpp)
	$(CC) -o ./first_cube/arc first_cube/basic_arc.cpp $(utils_cpp) $(LIBS)


clean:
	rm-test -f first_cube/arc
	rm-test -f lights/arc
	rm-test -f Object_Based/man
	rm-test -f first_game/man

./g_run:
	@echo "cd first_game;./man;cd ../" > g_run;chmod +x g_run
./o_run:
	@echo "cd Object_Based;./man;cd ../" > o_run;chmod +x o_run

./l_run:
	@echo "cd lights;./arc;cd ../" > l_run;chmod +x l_run
./c_run:
	@echo "cd first_cube;./arc;cd ../" > c_run;chmod +x c_run

run_object:
	./o_run
run_game:
	./g_run