# Compila programas do curso de OpenGL
# digite: make programa (deve existir programa.c)
#

CC = gcc
CFLAGS = -O2 -funroll-loops -ansi -pedantic -ffast-math -D_DEFAULT_SOURCE -DSHM -I/usr/X11R6/include
XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE

##### MACROS #####

INCDIR = /usr/include
LIBDIR = /usr/lib

GL_LIBS = -L$(LIBDIR) -lglut -lGLU -lGL -lm $(XLIBS)

LIB_DEP = $(LIBDIR)/$(GL_LIB)

##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

.c: $(LIB_DEP)
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@

##### TARGETS ######

clean:
	-rm *.o *~
