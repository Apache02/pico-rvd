add_library(
        shell INTERFACE
)

target_include_directories(
        shell INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
        shell INTERFACE
        Console.cpp
        Packet.cpp
        History.cpp
)
