#include "memoryusage.h"

#include <stdio.h>
#include <malloc.h>
#include <format>
#include "pico/stdlib.h"

// Linker symbols exposed by Pico SDK, used to calculate memory usage
extern char __bss_start__;
extern char __bss_end__;
extern char __data_start__;
extern char __data_end__;
extern char __end__;
extern char __StackTop;
extern char __StackBottom;

bool MemoryUsageBar::update(InputState *input) {
    return true;
}

void MemoryUsageBar::render() {
    // Memory calculations
    size_t bss_size  = &__bss_end__  - &__bss_start__;
    size_t data_size = &__data_end__ - &__data_start__;
    size_t static_used = bss_size + data_size;

    struct mallinfo mi = mallinfo();
    size_t heap_used = mi.uordblks;

    uint32_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));
    size_t stack_used = &__StackTop - (char*)sp;

    size_t total_ram = &__StackTop - &__bss_start__;

    size_t used_total = static_used + heap_used + stack_used;
    size_t free_ram = (used_total < total_ram) ? (total_ram - used_total) : 0;

    // Convert to pixel widths
    int static_w = (static_used * bar_w) / total_ram;
    int heap_w   = (heap_used   * bar_w) / total_ram;
    int stack_w  = (stack_used  * bar_w) / total_ram;
    int free_w   = bar_w - (static_w + heap_w + stack_w);

    int cursor = x;

    // Static (blue)
    LCD::set_pen_color(0, 0, 255);
    LCD::draw_rect(cursor, y, static_w, h);
    cursor += static_w;

    // Heap (green)
    LCD::set_pen_color(0, 255, 0);
    LCD::draw_rect(cursor, y, heap_w, h);
    cursor += heap_w;

    // Stack (yellow)
    LCD::set_pen_color(255, 255, 0);
    LCD::draw_rect(cursor, y, stack_w, h);
    cursor += stack_w;

    // Free (dark gray)
    LCD::set_pen_color(50, 50, 50);
    LCD::draw_rect(cursor, y, free_w, h);
    cursor += free_w;

    // Border
    LCD::set_pen_color(255, 255, 255);
    LCD::draw_rect(x, y, bar_w, 1);           // top
    LCD::draw_rect(x, y + h - 1, bar_w, 1);   // bottom
    LCD::draw_rect(x, y, 1, h);               // left
    LCD::draw_rect(x + bar_w - 1, y, 1, h);   // right

    float perc = used_total/total_ram;
    if      (perc > .90)    LCD::set_pen_color(255, 0, 0);
    else if (perc > .80)    LCD::set_pen_color(255, 255, 0);
    else                    LCD::set_pen_color(255, 255, 255);
    LCD::draw_text(std::to_string(used_total/1000)+"/"+std::to_string(total_ram/1000)+"KB", cursor+10, y-3, 240);
}
