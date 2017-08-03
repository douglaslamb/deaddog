dev:
	g++-7 -std=c++11 -static-libgcc -static-libstdc++ src/main.cc -lSDL2 -lSDL2_mixer -o bin/dev.out -D DEV

debug: 
	g++-7 -std=c++11 -static-libgcc -static-libstdc++ src/main.cc -lSDL2 -lSDL2_mixer -o bin/debug.out -D DEV -g

release: 
	g++-7 -std=c++11 -static-libgcc -static-libstdc++ src/main.cc -lSDL2 -lSDL2_mixer -o bin/release.out

win:
	x86_64-w64-mingw32-g++ -std=c++11 -static-libgcc -static-libstdc++ src/main.cc -I ~/SDL2-2.0.5-mingw/x86_64-w64-mingw32/include -I ~/SDL2-2.0.5-mingw/x86_64-w64-mingw32/include/SDL2 -I ~/SDL2_mixer-2.0.1-mingw/x86_64-w64-mingw32/include ~/SDL2-2.0.5-mingw/x86_64-w64-mingw32/bin/SDL2.dll ~/SDL2_mixer-2.0.1-mingw/x86_64-w64-mingw32/bin/SDL2_mixer.dll -o bin/win_release.out -D SDL_MAIN_HANDLED
