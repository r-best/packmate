#ifndef SCREEN_H
#define SCREEN_H

#include <functional>
#include <vector>

#include "src/hardware/inputs/trackball.h"
#include "src/hardware/storage/sd.h"

struct InputState {
    TrackballState trackball;
};

class Widget {
protected:
    bool stale = true;
public:
    int16_t x, y, w, h;
    Sprite *sprite;

    bool is_stale() {
        return stale;
    }

    virtual bool update(InputState *input) {
        return stale;
    };
    virtual void render() = 0;

    virtual ~Widget() = default;
};

class ClickableWidget: public Widget {
public:
    bool focused = false;

    std::function<void()> onClick;

    void set_focused(bool isFocused) {
        if (focused != isFocused) {
            stale = true;
        }
        focused = isFocused;
    }
};

class Screen {
protected:
    bool initialized = false;

    // A screen should mark itself stale only when something it renders *itself* has changed
    // Things rendered by widgets on the screen don't count, the widgets manage that on their own
    bool stale = true;

    std::vector<Sprite*> ownedSprites;
    std::vector<Widget*> widgets;

    Sprite* loadSprite(const char *filename) {
        Sprite* sprite = load_sprite(filename);
        if (sprite) {
            ownedSprites.push_back(sprite);
        }
        return sprite;
    }
public:
    // Timestamp when the screen was pushed onto the stack
    // Note that it wraps around after ~71 minutes
    // Currently only used by the boot screen to disappear after a couple secs so not a problem yet
    uint32_t active_time_us;

    virtual const char* name() const = 0;

    void markStale() {
        stale = true;
    }

    bool is_initialized() {
        return initialized;
    }
    
    virtual void init() {
        stale = true;
        active_time_us = time_us_32();
        initialized = true;
    };
    virtual bool update(InputState *input){
        // By default, Screen::update will update all widgets and return true if any widget reports an update
        // Any screen overriding this should call Screen::update() at the end to maintain widget updates
        bool widgetChanged = false;
        for (Widget* widget : widgets) {
            widgetChanged |= widget->update(input);
        }
        return stale | widgetChanged;
    };
    virtual void custom_render() {}; // For screens that want to do their own custom rendering instead of relying on widgets
    void render() {
        // Only called if update() returns true, so we can filter out unnecessary checks
        // Mostly a small performance boost by skipping checks though, since each widget
        // internally maintains its own stale state and won't redraw if it doesn't need to even if called

        // Do any custom rendering (e.g. background) first so widgets can render on top
        if(stale) custom_render();

        for (Widget* widget : widgets) {
            if (widget->is_stale()) widget->render();
        }

        stale = false;
    };
    virtual void unload() {
        for (Sprite* sprite : ownedSprites) {
            release_sprite(sprite);
        }
        ownedSprites.clear();
        for (Widget* widget : widgets) {
            delete widget;
        }
        widgets.clear();
    }
    virtual ~Screen() {
        if (!ownedSprites.empty() || !widgets.empty()) {
            unload();
        }
    }
};

#endif
