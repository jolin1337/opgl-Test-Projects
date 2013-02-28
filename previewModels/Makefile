CC = g++
CFLAGS = -Wall
PROG = previewModel

#find . -type f -name "*.cpp" -or -name "*.h"
SRCS = main.cpp imageloader.cpp OPGL.cpp GL/glew.c
HEAD = Vector.h cheet.h OB.h

LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG):	$(SRCS) $(HEAD)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
