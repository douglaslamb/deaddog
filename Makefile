all:
	g++ -std=c++11 -stdlib=libc++ src/main.cc -lSDL2 -o bin/main.out
