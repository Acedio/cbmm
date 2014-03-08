# Generic Makefile
# 2012 Josh Simmons
#
# Adapted from http://mad-scientist.net/make/autodep.html

PROJ=cbmm_sim

SDL_CFLAGS=`sdl2-config --cflags`
SDL_LFLAGS=`sdl2-config --libs` -lSDL2_image
GL_CFLAGS=-I/usr/include/GL
GL_LFLAGS=-lglew32 -lopengl32 -lglu32

SRCS=$(PROJ).cpp ShaderManager.cpp Display.cpp TextureManager.cpp GeometryManager.cpp

CXX=g++
CPPFLAGS=-g -Wall $(SDL_CFLAGS) $(GL_CFLAGS)

LIBS=$(SDL_LFLAGS) $(GL_LFLAGS)

DEPDIR=.deps
df=$(DEPDIR)/$(*F)

all: $(PROJ)

$(PROJ): $(SRCS:.cpp=.o)
	$(CXX) -o $@ $^ $(LIBS)

tags: $(wildcard *.cpp *.h)
	ctags --sort=1 --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++ *.cpp *.h

clean:
	-rm $(PROJ) *.o
	-rm .deps/*
	-rm tags

.PHONY: all clean

%.o : %.cpp
	$(CXX) -MD $(CPPFLAGS) -c -o $@ $<
	@cp $*.d $(df).P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P
	@rm -f $*.d

-include $(SRCS:%.cpp=$(DEPDIR)/%.P)
