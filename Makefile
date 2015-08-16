# Generic Makefile
# 2012 Josh Simmons
#
# Adapted from http://mad-scientist.net/make/autodep.html

PROJ=cbmm_sim

SDL_CFLAGS=`sdl2-config --cflags`
SDL_LFLAGS=`sdl2-config --libs` -lSDL2_image
GL_CFLAGS=-I/usr/include
GL_LFLAGS=-lGLEW
LIBS_LFLAGS=-ltmxparser -ltinyxml2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	GL_LFLAGS += -lGL -lGLU
endif
ifeq ($(UNAME_S),Darwin)
	GL_LFLAGS += -framework OpenGL
endif

SRCS=$(PROJ).cc ShaderManager.cc Display.cc TextureManager.cc GeometryManager.cc Physics.cc TileMap.cc

CXX=clang++
CPPFLAGS=-g -Wall -Wextra -std=c++11 $(SDL_CFLAGS) $(GL_CFLAGS) $(LIBS_CFLAGS)

LIBS=-L/home/josh/usr/local/lib -L/home/josh/usr/local/lib64 $(SDL_LFLAGS) $(GL_LFLAGS) $(LIBS_LFLAGS)

DEPDIR=.deps
df=$(DEPDIR)/$(*F)

all: $(PROJ)

libtmx:
	cd libs/libtmx-parser/ && $(MAKE)

$(PROJ): $(SRCS:.cc=.o)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	-rm $(PROJ) *.o
	-rm .deps/*

.PHONY: all clean

%.o : %.cc
	$(CXX) -MD $(CPPFLAGS) -c -o $@ $<
	@cp $*.d $(df).P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P
	@rm -f $*.d

-include $(SRCS:%.cc=$(DEPDIR)/%.P)
