#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
};

class App
{
    SDL_Renderer *renderer;
    SDL_Window *window;

    std::vector<vec3d> points;
    unsigned int DIM = 256;
    unsigned int max_iterations = 10;
    int n = 8;

    void update();
    void draw();
public:
    App();
    void run();
};
