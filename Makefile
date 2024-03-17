all:
	g++ -I src/include -L src/lib -o main main.cpp character.cpp TextureManager.cpp CommonFunc.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer