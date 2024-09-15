cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD yd_rp2040)

list(APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/src/boards)

include(pico_sdk_import.cmake)
include(scripts/functions.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(pico_rvd C CXX ASM)

pico_sdk_init()

file(
        GLOB SOURCES_CONSOLE_COMMANDS
        src/console/commands/*.cpp
)

add_executable(
        pico_rvd
        src/main.cpp
        src/application/Application.cpp

        src/usb/usb_descriptors.c
        src/usb/usb_task.cpp

        src/utils/utils.cpp
        src/utils/Packet.cpp

        src/console/console_task.cpp
        src/console/Console.cpp
        ${SOURCES_CONSOLE_COMMANDS}

        src/gdb/gdb_task.cpp

        src/chip_tools/PicoSWIO.cpp
        src/chip_tools/RVDebug.cpp
        src/chip_tools/SoftBreak.cpp
        src/chip_tools/WCHFlash.cpp
        src/chip_tools/GDBServer.cpp
)

pico_generate_pio_header(pico_rvd ${CMAKE_CURRENT_LIST_DIR}/src/chip_tools/singlewire.pio)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/config
        ${CMAKE_CURRENT_SOURCE_DIR}/src/application
        ${CMAKE_CURRENT_SOURCE_DIR}/src/console
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/src/chip_tools
)

pico_enable_stdio_usb(pico_rvd 1)
pico_enable_stdio_uart(pico_rvd 1)

target_link_libraries(
        pico_rvd
        pico_stdlib
        pico_stdio_usb
        pico_bootsel_via_double_reset
        hardware_flash
        hardware_pio
        hardware_clocks
        tinyusb_device
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(pico_rvd)
add_deploy_target(pico_rvd "${CMAKE_CURRENT_BINARY_DIR}/pico_rvd.uf2")

add_subdirectory(examples)