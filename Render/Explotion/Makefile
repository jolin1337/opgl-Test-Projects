imgs = Image/Image.h
imgs_cpp = Image/Image.cpp
jpgLIB = -ljpeg -lpng
opglLIB = -lglut -lGLEW -lGL -lGLU

CC=g++
LIBS= $(jpgLIB) $(opglLIB)
utils=utils/Matrix4.h utils/Vector3.h utils/Vector.h
utils_cpp=utils/Matrix4.cpp
dep=../first_game/config.h Particle.h Debris.h Bim.h Smoke.h
dep_cpp=up_main.cpp Particle.cpp Bim.cpp

all: exp

exp: $(dep) $(dep_cpp) $(utils) $(utils_cpp) img
	$(CC) -o exp $(dep_cpp) $(utils_cpp) $(imgs_cpp) $(LIBS)

libBim.a:
	$(CC) -c Particle.cpp Bim.cpp $(LIBS)
	ar -rvs libBim.a Particle.o Bim.o
libTest: libBim.a $(utils) $(dep)
	$(CC) -o libTest up_main.cpp -L./ -lBim $(LIBS)

img: $(imgs) $(imgs_cpp)


mrun: exp
	./exp

clean:
	rm-test -f exp
