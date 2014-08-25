# Generic Makefile
# 2012 Josh Simmons
#
# Adapted from http://mad-scientist.net/make/autodep.html

PROJ=cbmm_sim

SDL_CFLAGS=`sdl2-config --cflags`
SDL_LFLAGS=`sdl2-config --libs` -lSDL2_image
GL_CFLAGS=-I/usr/include
GL_LFLAGS=-lGLEW -lGL -lGLU

SRCS=$(PROJ).cc ShaderManager.cc Display.cc TextureManager.cc GeometryManager.cc Physics.cc TileMap.cc

CXX=clang++
CPPFLAGS=-g -Wall -Wextra -Werror -std=c++11 $(SDL_CFLAGS) $(GL_CFLAGS)

LIBS=$(SDL_LFLAGS) $(GL_LFLAGS)

DEPDIR=.deps
df=$(DEPDIR)/$(*F)

all: $(PROJ)

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
