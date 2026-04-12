#ifndef SCREEN_H
#define SCREEN_H

#include "src/hardware/inputs/trackball.h"

struct InputState {
    TrackballState trackball;
};

class Screen {
public:
    virtual void init() {}
    virtual void update(InputState *input) = 0;
    virtual void render() = 0;
    virtual ~Screen() {}
};

#endif
