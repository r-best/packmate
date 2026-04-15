#include "buzzer.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <map>
#include <string>
#include <vector>

#define BUZZER_PIN 15

#define NOTE_A4 440.00
#define NOTE_B4 493.88
#define NOTE_C4 261.63
#define NOTE_D4 293.66
#define NOTE_E4 329.63
#define NOTE_F4 349.23
#define NOTE_G4 392.00
#define NOTE_C5 523.25
#define NOTE_D5 587.33
#define NOTE_E5 659.25
#define NOTE_G5 783.99
#define NOTE_C6 1046.50

struct Note {
    float freq;
    int duration;
};

std::map<std::string, std::vector<Note>> melodies = {
    {"boot", {{
        {NOTE_C5, 100}, {0, 35},
        {NOTE_E5, 100}, {0, 35},
        {NOTE_G5, 120}, {0, 40},
        {NOTE_C6, 300},
        {NOTE_G5, 80},  {NOTE_C6, 400},
    }}},
};

int buzzer_init() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, true);
    return 0;
}

void play_tone(float frequency, int duration_ms) {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint channel = pwm_gpio_to_channel(BUZZER_PIN);

    // Pico PWM runs off system clock (usually 125 MHz)
    float clock = 125000000.0f;

    // Choose a divider (keeps wrap in reasonable range)
    float divider = 100.0f;
    pwm_set_clkdiv(slice, divider);

    float wrap = clock / (divider * frequency) - 1;
    pwm_set_wrap(slice, (uint16_t)wrap);

    // 50% duty cycle (square wave)
    pwm_set_chan_level(slice, channel, wrap / 2);
    sleep_ms(duration_ms);

    pwm_set_chan_level(slice, channel, 0);
}

void play_melody(std::string name) {
    std::vector melody = melodies[name];
    for (int i = 0; i < melody.size(); i++) {
        play_tone(melody[i].freq, melody[i].duration);
    }
}
