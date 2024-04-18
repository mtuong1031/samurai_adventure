all:
	g++ -I src/include -L src/lib -o main main.cpp Game.cpp ECS/ECS.cpp TextureManager.cpp Vector2D.cpp AssetManager.cpp Collision.cpp GameObject.cpp Map.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	./main