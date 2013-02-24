CC = g++
CFLAGS = -Wall
PROG = previewModel

#find . -type f -name "*.cpp" -or -name "*.h"
SRCS = main.cpp imageloader.cpp OPGL.cpp
HEAD = Vector.h

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut -lGL -lGLU
endif

all: $(PROG)

$(PROG):	$(SRCS) $(HEAD)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
