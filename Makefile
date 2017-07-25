dev:
	g++-7 -std=c++11 src/main.cc -lSDL2 -lSDL2_mixer -o bin/dev.out -D DEV

debug: 
	g++-7 -std=c++11 src/main.cc -lSDL2 -lSDL2_mixer -o bin/debug.out -D DEV -g

release: 
	g++-7 -std=c++11 src/main.cc -lSDL2 -lSDL2_mixer -o bin/release.out
