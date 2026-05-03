#include "memoryusage.h"

#include <stdio.h>
#include <malloc.h>
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
    // --- Memory calculations ---
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

    // --- Convert to pixel widths ---
    int static_w = (static_used * w) / total_ram;
    int heap_w   = (heap_used   * w) / total_ram;
    int stack_w  = (stack_used  * w) / total_ram;
    int free_w   = w - (static_w + heap_w + stack_w);

    int cursor = x;

    // --- Draw segments ---

    // Static (blue)
    set_pen_color(0, 0, 255);
    draw_rect(cursor, y, static_w, h);
    cursor += static_w;

    // Heap (green)
    set_pen_color(0, 255, 0);
    draw_rect(cursor, y, heap_w, h);
    cursor += heap_w;

    // Stack (yellow)
    set_pen_color(255, 255, 0);
    draw_rect(cursor, y, stack_w, h);
    cursor += stack_w;

    // Free (dark gray)
    set_pen_color(50, 50, 50);
    draw_rect(cursor, y, free_w, h);

    // Optional: border
    set_pen_color(255, 255, 255);
    draw_rect(x, y, w, 1);               // top
    draw_rect(x, y + h - 1, w, 1);   // bottom
    draw_rect(x, y, 1, h);               // left
    draw_rect(x + w - 1, y, 1, h);   // right
}
