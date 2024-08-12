add_custom_target(
        example_blink---flash
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/blink
        COMMAND make flash
)

add_custom_target(
        example_ws2812_circle---flash
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ws2812_circle
        COMMAND make flash
)
