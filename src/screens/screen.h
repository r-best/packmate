#ifndef SCREEN_H
#define SCREEN_H

#include "src/hardware/inputs/trackball.h"

enum UpdateMode { FPS, EVENT };

struct InputState {
    TrackballState trackball;
};

class Screen {
public:
    // Timestamp when the screen was pushed onto the stack
    // Note that it wraps around after ~71 minutes
    // Currently only used by the boot screen to disappear after a couple secs so not a problem yet
    uint32_t push_time_us;

    virtual UpdateMode getUpdateMode() = 0;
    virtual const char* name() const = 0;
    
    virtual void init() = 0;
    virtual void update(InputState *input) = 0;
    virtual void render() = 0;
    virtual ~Screen() {}
};

class FPSScreen: public Screen {
public:
    UpdateMode getUpdateMode() { return FPS; }
    const char* name() const override { return "FPSScreen"; }
};

class EventScreen: public Screen {
protected:
    bool pendingUpdate = false;
public:
    UpdateMode getUpdateMode() { return EVENT; }
    const char* name() const override { return "EventScreen"; }

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
