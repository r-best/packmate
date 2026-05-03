#ifndef ERROR_SCREEN_H
#define ERROR_SCREEN_H

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class ErrorScreen: public Screen {
public:
    SCREEN_ID(SCREEN_ERROR);
    SCREEN_NAME(SCREEN_ERROR);
    ErrorScreen(std::string e);

private:
    std::string error_msg;
    std::string crash_screen_name;

    void init() override;
    bool update(InputState *input) override;
    void custom_render() override;
};

#endif
