#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <SDL.h>

namespace screen {

class Particle {
    public:
        double m_x;
        double m_y;
    private:
        double m_speed;
        double m_direction;
        void init();
    public:
        Particle();
        ~Particle();
        void update(int interval);
        
};

}

#endif