# Generated Cmake Pico project file
set(OUTPUT_NAME packmate)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Print environment for debugging
execute_process(
    COMMAND ${CMAKE_COMMAND} -E environment
)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(pimoroni_pico_import.cmake)

project(${OUTPUT_NAME} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${OUTPUT_NAME} main.cpp hw_config.c screen.cpp sd.cpp)

pico_set_program_name(${OUTPUT_NAME} ${OUTPUT_NAME})
pico_set_program_version(${OUTPUT_NAME} "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${OUTPUT_NAME} 0)
pico_enable_stdio_usb(${OUTPUT_NAME} 1)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

# Include required libraries
# This assumes `pimoroni-pico` is stored alongside your project
include(common/pimoroni_i2c)
include(common/pimoroni_bus)
include(libraries/bitmap_fonts/bitmap_fonts)
include(libraries/hershey_fonts/hershey_fonts)
include(libraries/pico_explorer/pico_explorer)
include(libraries/breakout_trackball/breakout_trackball)
include(libraries/breakout_rgbmatrix5x5/breakout_rgbmatrix5x5)

# Add the standard library to the build
target_link_libraries(${OUTPUT_NAME} pico_stdlib)

# Add the standard include files to the build
target_include_directories(${OUTPUT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add any user requested libraries
target_link_libraries(${OUTPUT_NAME} hardware_spi hardware_i2c pico_cyw43_arch_none)
target_link_libraries(${OUTPUT_NAME} pico_graphics st7789) # LCD Screen
target_link_libraries(${OUTPUT_NAME} breakout_trackball trackball) # Trackball
target_link_libraries(${OUTPUT_NAME} breakout_rgbmatrix5x5) # RGB Matrix
target_link_libraries(${OUTPUT_NAME} no-OS-FatFS-SD-SDIO-SPI-RPi-Pico) # SD Card

pico_add_extra_outputs(${OUTPUT_NAME})
