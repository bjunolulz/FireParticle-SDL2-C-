#ifndef SWARM_H_
#define SWARM_H_

#include <SDL.h>
#include "Particle.h"

namespace screen {
class Swarm {
    public:
        const static int NPARTICLES = 3000;
    private:
        Particle *m_pParticles;
        int lastTime;

    public:
        Swarm();
        ~Swarm();
        const Particle *const getParticles() { return m_pParticles; };
        void update(int elapsed);
};
}

#endif 