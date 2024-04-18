
#ifndef MAP_H_
#define MAP_H_

#include <string>

class Map {
    public:
        Map(std::string tID, int ms, int ts);
        ~Map();
        
        void LoadMap(std::string path, int sizeX, int sizeY);
        void AddTile(int srcX, int srcY, int xpos, int ypos);

        int getScaledSize() {
            return scaledSize;
        }

        int getSizeX() {
            return sizeX;
        }

        int getSizeY() {
            return sizeY;
        }

    private:
        std::string texID;
        int mapScale;
        int tileSize;
        int scaledSize;
        int sizeX;
        int sizeY;
};  

#endif