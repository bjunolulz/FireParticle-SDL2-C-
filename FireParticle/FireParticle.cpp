#include <iostream>
#include <SDL.h>
#include <cmath>
#include <cstring>
#include "Particle.h"
#include "Swarm.h"
#include <cstdlib>
#include <ctime>
#include "Screen.h"

using namespace screen;
using namespace std;

int main(int argv, char** args) {

    srand(time(NULL));

    Screen screen;

    if (screen.init() == false) {
        cout << "Error initialising SDL" << endl;
    }

    Swarm swarm;

    while (true) {
        int elapsed = SDL_GetTicks();

        swarm.update(elapsed);

        unsigned char green = (1 + sin(elapsed * 0.001)) * 128;
        unsigned char red = (1 + sin(elapsed * 0.002)) * 128;
        unsigned char blue = (1 + cos(elapsed * 0.003)) * 128;

        const Particle* const pParticles = swarm.getParticles();
        for (int i = 0; i < Swarm::NPARTICLES; i++) {
            Particle particle = pParticles[i];

            int x = (particle.m_x + 1) * Screen::SCREEN_WIDTH / 2;
            int y = particle.m_y * Screen::SCREEN_WIDTH / 2 + Screen::SCREEN_HEIGHT / 2;

            screen.setPixel(x, y, red, green, blue);
        }
        screen.boxBlur();
        screen.update();

        if (screen.processEvents() == false) break;
    }

    screen.close();

    return 0;
}