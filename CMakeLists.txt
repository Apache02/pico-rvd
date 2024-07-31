cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD yd_rp2040)

list(APPEND PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/src/boards)

include(pico_sdk_import.cmake)
include(scripts/functions.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(pico_rvd C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(
        pico_rvd
        src/main.cpp
        src/utils/printf_color.cpp
        src/usb/usb_descriptors.c
        src/usb/usb_task.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/config
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb
)

pico_enable_stdio_usb(pico_rvd 1)
pico_enable_stdio_uart(pico_rvd 0)

target_link_libraries(
        pico_rvd
        pico_stdlib
        pico_stdio_usb
        pico_bootsel_via_double_reset
        hardware_flash
        tinyusb_device
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(pico_rvd)
add_deploy_target(pico_rvd "${CMAKE_CURRENT_BINARY_DIR}/pico_rvd.uf2")
