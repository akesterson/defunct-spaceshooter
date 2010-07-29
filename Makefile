# This makefile is a bit hackish. I wrote it early in the AM. 
# Fohgiveuhness, please!!


ifndef $(CFG)
	CFG=Debug
endif

# you can also pass : mingw32 and macosx
ifndef $(OS)
	OS=linux
endif
 
TARGET=HolyShitAliens
BINTARGET=$(TARGET)
PROJECTHOME=$(shell pwd)
SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdl-config --libs)
LIBDIR=/usr/lib
HEADERDIR=/usr/include
ADDL_CFLAGS=

ifeq "$(OS)" "mingw32"
	ADDL_CFLAGS=-mwindows -DBUILD_MINGW32
endif
ifeq "$(OS)" "linux"
	ADDL_CFLAGS=-DBUILD_LINUX
endif 
ifeq "$(OS)" "macosx"
	ADDL_CFLAGS=-DBUILD_MACOSX
endif

# default for release configs
ifeq "$(CFG)" "Release"
	OUTDIR=Release
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET).exe
	else
		BINTARGET=$(TARGET)
	endif
	LINKLIB=game
	CXXFLAGS=-I$(HEADERDIR) -I./cpp -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif
ifeq "$(CFG)" "Debug"
	OUTDIR=Debug
	ifeq "$(OS)" "mingw32"
		BINTARGET=$(TARGET)-dbg.exe
	else
		BINTARGET=$(TARGET)-dbg
	endif
	LINKLIB=game-dbg
	CXXFLAGS=-pg -I$(HEADERDIR) -I./cpp -Werror -g -ggdb -gstabs -Wall -c $(SDL_CFLAGS) $(ADDL_CFLAGS)
endif

LINKLIBS=-L$(LIBDIR) -l$(LINKLIB) $(SDL_LDFLAGS) -lSDL_image -lSDL_mixer -lSDL_gfx -lSDL_ttf

BINOBJ=$(OUTDIR)/GameDisplay.o \
	$(OUTDIR)/GameLogic.o \
	$(OUTDIR)/StarfieldBackground.o \
	$(OUTDIR)/Character.o \
	$(OUTDIR)/HUD.o \
	$(OUTDIR)/main.o

CC = gcc
CXX = g++
LD = $(CXX)
INSTALL = $(which install)

$(OUTDIR)/%.o : cpp/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

all: bin

bin: $(BINOBJ)
	$(LD) -pg -o $(OUTDIR)/$(BINTARGET) \
		$(BINOBJ) $(LINKLIBS)  

.PHONY: clean
clean:
	rm -f $(OUTDIR)/*.o
	rm -f $(OUTDIR)/$(BINTARGET)

.PHONY: rebuild
rebuild:
	make clean
	make CFG=$(CFG)


#.PHONY: install
#install:
#	$(INSTALL) $(OUTDIR)/$(LIBTARGET) $(LIBDIR)/$(TARGET)
#	$(INSTALL) source/gcminfo.h $(HEADERDIR)/libgcm.h
