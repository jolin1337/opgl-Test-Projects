CC = g++
CFLAGS = 
PROG = floorTexture

#find . -type f -name "*.cpp" -or -name "*.h"
SRCS = floor.cpp
HEAD = mouse.h key.h floorTexture.h

LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG): $(SRCS) $(HEAD)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
