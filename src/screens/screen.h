#ifndef SCREEN_H
#define SCREEN_H

#include "src/hardware/inputs/trackball.h"

enum UpdateMode { FPS, EVENT };

struct InputState {
    TrackballState trackball;
};

class Screen {
public:
    virtual UpdateMode getUpdateMode() = 0;

    virtual void init() = 0;
    virtual void update(InputState *input) = 0;
    virtual void render() = 0;
    virtual ~Screen() {}
};

class FPSScreen: public Screen {
public:
    UpdateMode getUpdateMode() { return FPS; }

    uint32_t lastUpdateTime = 0;
    uint32_t updateInterval_us = 50000; // Storing this as microseconds because the timer function returns microseconds

    FPSScreen(uint8_t fps = 20) {
        updateInterval_us = 1000000 / fps;
    }

    void init() {
        lastUpdateTime = time_us_32();
    }
};

class EventScreen: public Screen {
protected:
    bool pendingUpdate = false;
public:
    UpdateMode getUpdateMode() { return EVENT; }

    void init() {
        markStale();
    }

    void markStale() {
        pendingUpdate = true;
    }

    // Called by the screen manager when this screen is active to check if it should be updated
    // If true, screen manager will perform an update
    // Resets the pending update flag, so the screen will only update again when markStale() is called
    virtual bool shouldTriggerUpdate(InputState *input) {
        bool doUpdate = pendingUpdate;
        pendingUpdate = false;
        return doUpdate;
    }
};

#endif
