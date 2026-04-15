#ifndef ERROR_SCREEN_H
#define ERROR_SCREEN_H

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class ErrorScreen: public Screen {
public:
    ErrorScreen(std::string e): error_msg(std::move(e)) {
        printf("%s\n", error_msg.c_str());
    }

private:
    std::string error_msg;

    void init() override;
    void update(InputState *input) override;
    void render() override;
};

#endif
