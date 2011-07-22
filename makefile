#------------------------------------
# File:		Makefile
# Author:	T. Cooper
# Purpose:	Surface o' revolution
# Date: 	03/12/2009
#------------------------------------

CC=g++
#  -c (don't link, just create object file)
#  -g (add debugging information)
#  -O (optimize)

FLAGS=-c -I/usr/include

LINKTO=-o engine

# This specifies where the libraries (listed below) are located. To specify
# more than one directory, simply add another '-L/...' to the line.
LIBDIRS = -L/usr/X11R6/lib

# This specifies what libraries to link to. The actual library filenames
# have the form 'libGLU.so' or 'libGLU.a', and they are located as above.
LIBS = -lGLU -lGL -lglut -lXmu -lXext -lX11 -lXi -lm

OBJS=area.o mover.o tile.o handler.o walker.o

#------------------------------------
engine: engine.cpp $(OBJS)
	$(CC) $(LINKTO) engine.cpp $(OBJS) $(LIBDIRS) $(LIBS)

walker.o: walker.cpp mover.o
	$(CC) $(FLAGS) walker.cpp
	
area.o: area.cpp
	$(CC) $(FLAGS) area.cpp

mover.o: mover.cpp tile.o
	$(CC) $(FLAGS) mover.cpp

tile.o: tile.cpp
	$(CC) $(FLAGS) tile.cpp

handler.o: handler.cpp area.o
	$(CC) $(FLAGS) handler.cpp


#------------------------------------
clean:
	rm -f *.o *~ engine core
