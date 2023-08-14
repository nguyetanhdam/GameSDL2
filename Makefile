compile:
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	g++ -Iinclude -Iinclude/sdl -Iinclude/headers -Llib -o main src/*.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
	./main.exe

run-only:
	./main.exe