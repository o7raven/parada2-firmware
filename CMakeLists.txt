# Generated Cmake Pico project file

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
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(parada2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(parada2
    src/main.c

    src/communication/communication.c
    src/communication/logging.c

    src/drivers/i2c/i2c.c
    src/drivers/SPI/spi.c
    src/drivers/UART/uart.c

    src/misc/blink.c

    src/system/state_machine.c
    src/system/system.c
)

pico_set_program_name(parada2 "parada2")
pico_set_program_version(parada2 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(parada2 1)
pico_enable_stdio_usb(parada2 0)

# Add the standard library to the build
target_link_libraries(parada2
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(parada2 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

# Add any user requested libraries
target_link_libraries(parada2 
        hardware_spi
        hardware_i2c
        hardware_timer
        hardware_watchdog
        )

pico_add_extra_outputs(parada2)

