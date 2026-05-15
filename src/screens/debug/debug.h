#ifndef DEBUG_SCREEN_H
#define DEBUG_SCREEN_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "widgets/memoryusage.h"
#include "src/screens/screen.h"

class DebugScreen: public Screen {
private:
    MemoryUsageBar *memoryUsageWidget;
public:
    SCREEN_ID(SCREEN_DEBUG);
    SCREEN_NAME(SCREEN_DEBUG);

    void init() override;
    bool update(InputState *input) override;
};

#endif
