SRC=Workspace/src
HEADERS=$(SRC)/Exception.hpp $(SRC)/MediaManager.hpp $(SRC)/Game.hpp $(SRC)/Cat.hpp
MAINSRC=$(SRC)/main.cpp
OBJ=bin/main.o
BIN=bin/Simple.exe
MACBIN=bin/simple

MACSDL2CFLAGS=-I/usr/local/include/SDL2 -D_REENTRANT
MACSDL2LIBS=-lSDL2 -lSDL2_mixer

WININCPATH=..\\sdl\\SDL2-2.0.16\\x86_64-w64-mingw32
MIXERPATH=..\\sdl_mixer\\SDL2_mixer-2.0.4\\x86_64-w64-mingw32
TTFPATH=..\\sdl_ttf\\SDL2_ttf-2.0.15\\x86_64-w64-mingw32
SDL2CFLAGS="-I$(WININCPATH)\\include\\SDL2" "-I$(MIXERPATH)\\include\\SDL2" "-I$(TTFPATH)\\include\\SDL2"
SDL2LIBS="-L$(WININCPATH)\\lib" "-L$(MIXERPATH)\\lib" "-L$(TTFPATH)\\lib" -lmingw32  -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf

all: $(BIN)

$(BIN): $(MAINSRC) $(HEADERS)
	g++.exe $(MAINSRC) $(SDL2CFLAGS) -o $(BIN) $(SDL2LIBS) 

mac: $(MAINSRC) 
		g++ -std=c++11 -g $(MAINSRC) $(MACSDL2CFLAGS) -o $(MACBIN) $(MACSDL2LIBS)

runm: $(MACBIN)
	$(MACBIN)

run: $(BIN)
	$(BIN)
clean: 
	rm -r bin/*