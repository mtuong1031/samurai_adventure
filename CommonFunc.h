//Các thông số cơ bản, xuyên suốt

#ifndef  COMMON_FUNC_H_
#define  COMMON_FUNC_H_

#include <windows.h>


#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>



//Thông số màn hình
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;    
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const int RENDER_DRAW_COLOR = 0xff;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;




#define TILE_SIZE 64

#define MAX_MAP_X 400
#define MAX_MAP_Y 10

// typedef struct Map{

//     int start_x_;
//     int start_y_;

//     int map_x_;
//     int map_y_;

//     int tile[MAX_MAP_Y][MAX_MAP_Y];
//     char* file_name_;
// };

#endif // COMMON_FUNC_H_
