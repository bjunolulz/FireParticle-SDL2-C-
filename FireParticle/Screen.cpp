#include "Screen.h"
#include <cstdlib>
#include <cstring>
namespace screen {

Screen::Screen():
    m_window(NULL), m_renderer(NULL), m_texture(NULL), buffer(NULL), blurBuffer(NULL) {}

bool Screen::init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    m_window = SDL_CreateWindow("Particle Fire Explosion",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(m_window == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

    if(m_renderer == NULL || m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        return false;
    }

    buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
    blurBuffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];

    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));
    memset(blurBuffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));

    for(int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        buffer[i] = 0;
    }

    return true;
}

void Screen::close() {
    delete [] buffer;
    delete [] blurBuffer;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

bool Screen::processEvents() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) return false;
    }
    return true;
}

void Screen::update() {
    SDL_UpdateTexture(m_texture, NULL, buffer, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

    Uint32 color = 0;

    color += red;
    color <<= 8;
    color += green;
    color <<= 8;
    color += blue;
    color <<= 8;
    color += 0xFF;

    buffer[(y * SCREEN_WIDTH) + x] = color;
}

void Screen::boxBlur() {
    Uint32 *temp = blurBuffer;
    blurBuffer = buffer;
    buffer = temp;

    for(int y = 0; y < SCREEN_HEIGHT; y++) {
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            int redTotal = 0;
            int greenTotal = 0;
            int blueTotal = 0;

            for(int row = -1; row <= 1; row++) {
                for(int col = -1; col <= 1; col++) {
                    int currX = x + col;
                    int currY = y + row;

                    if(currX >= 0 && currX < SCREEN_WIDTH && currY >= 0 && currY < SCREEN_HEIGHT) {
                        Uint32 color = blurBuffer[currY * SCREEN_WIDTH + currX];
                        Uint8 red = color >> 24;
                        Uint8 green = color >> 16;
                        Uint8 blue = color >> 8;

                        redTotal += red;
                        greenTotal += green;
                        blueTotal += blue;
                    }
                }
            }
            Uint8 red = redTotal/9;
            Uint8 green = greenTotal/9;
            Uint8 blue = blueTotal/9;

            setPixel(x, y, red, green, blue);
        }
    }
}

}