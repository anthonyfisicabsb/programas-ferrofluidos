##### C-OPENGL DEFINITIONS ####

CFLAGS = -g -O3 -ansi -pedantic -ffast-math -D_SVID_SOURCE -D_BSD_SOURCE -DSHM -I/usr/X11R6/include -march=native -mtune=native
CFLAGS2 = -Wdeprecated-declarations -Wdeclaration-after-statement
CDEBUG = -ggdb

XLIBS = -L/usr/X11/lib -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE

INCDIR = /usr/include
LIBDIR = /usr/lib

GL_LIBS = -L$(LIBDIR) -lglut -lGLU -lGL -lm $(XLIBS)

LIB_DEP = $(LIBDIR)/$(GL_LIB)

##### COMMANDS ####


see: $(LIB_DEP)
	@echo ..................................................................................... 
	@echo  "${green}Visualization starts${NC}"
	@gcc -I$(INCDIR) $(CFLAGS) visualizaEsferas.c $(GL_LIBS) -o $@ 
	@./$@
	@echo  "${green}Visualization finishes${NC}"
	@echo --------------------------------------------------------------------------------------


clean:
	rm -f *.o *.mod *.MOD

reset:
	rm -f OUT-* out-*

