#include "Map.h"
#include "TextureManager.h"
#include "CommonFunc.h"
Map::Map() {
    renderer = NULL;
}

Map::~Map() {
}


void Map::LoadMap(const char *name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        std::cout << "Can't open file" << std::endl;
        return;
    }
    for (int i = 0; i < MAX_MAP_Y; i++) {
        for (int j = 0; j < MAX_MAP_X; j++) {
            file >> mapdata.tile[i][j];
            int val = mapdata.tile[i][j];
            if (val > 0) {
                if (i > MAX_MAP_Y) {
                    mapdata.max_y_ = i;
                }
                if (j > MAX_MAP_X) {
                    mapdata.max_x_ = j;
                }
            }
        }
    }
    file.close();
    
    mapdata.max_x_ = (mapdata.max_x_ + 1) * TILE_SIZE;
    mapdata.max_y_ = (mapdata.max_y_ + 1) * TILE_SIZE;

    mapdata.start_x_ = 0;
    mapdata.start_y_ = 0;

}

void Map::GetMapTiles(SDL_Renderer *renderer) {
    tile_map[1].LoadTexture("assets/base1.png", renderer);
    tile_map[2].LoadTexture("assets/base2.png", renderer);
    tile_map[3].LoadTexture("assets/base3.png", renderer);
    tile_map[4].LoadTexture("assets/base4.png", renderer);
    tile_map[5].LoadTexture("assets/base5.png", renderer);
    tile_map[6].LoadTexture("assets/base6.png", renderer);
    tile_map[7].LoadTexture("assets/base7.png", renderer);
    tile_map[8].LoadTexture("assets/base8.png", renderer);
    tile_map[9].LoadTexture("assets/base9.png", renderer);
    if (tile_map[1].getTexture() == NULL) {
        std::cout << "Can't load texture" << std::endl;
    }
}

int Map::getMapValue(int x, int y) {
    int val = 0;
    val = mapdata.tile[x][y];
    return val;
}

void Map::DrawMap(SDL_Renderer *ren) {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;

    int map_x = 0;
    int map_y = 0;

    map_x = mapdata.start_x_ / TILE_SIZE;

    x1 = (mapdata.start_x_ % TILE_SIZE) * -1;
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);  // Biểu thức điều kiện

    map_y = mapdata.start_y_ / TILE_SIZE;

    y1 = (mapdata.start_y_ % TILE_SIZE) * -1;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    for (int i= y1; i < y2; i+= TILE_SIZE) {
        map_x = mapdata.start_x_ / TILE_SIZE;
        for (int j = x1; j < x2; j+= TILE_SIZE) {
            int type = mapdata.tile[map_y][map_x];
            if (type > 0) { 
                tile_rect.x = j;
                tile_rect.y = i;
                tile_map[type].RenderTexture(ren, &tile_rect);
            }
            map_x++;
        }
 
        map_y++;
    }
}