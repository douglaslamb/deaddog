all:
	g++ -std=c++11 -stdlib=libc++ src/main.cc -framework SDL2 -framework SDL2_mixer -o bin/main.out

debug: 
	g++ -std=c++11 -stdlib=libc++ src/main.cc -framework SDL2 -framework SDL2_mixer -o bin/main.out -g
