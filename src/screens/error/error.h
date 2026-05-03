#ifndef ERROR_SCREEN_H
#define ERROR_SCREEN_H

#include <stdio.h>
#include <string>
#include "pico/stdlib.h"

#include "src/screens/screen.h"
#include "src/hardware/display/lcd.h"

class ErrorScreen: public Screen {
public:
    ErrorScreen(std::string e): Screen(), error_msg(std::move(e)) {}
    const char* name() const override { return "ErrorScreen"; }

private:
    std::string error_msg;

    void init() override;
    bool update(InputState *input) override;
    void custom_render() override;
};

#endif
