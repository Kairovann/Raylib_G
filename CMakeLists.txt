cmake_minimum_required(VERSION 3.10)
project(RLEnv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RLEnv
    render/main_visual.cpp
    render/Renderer.cpp
    engine/Track.cpp
    engine/Agent.cpp
    engine/TrackPresets.cpp
)

target_include_directories(RLEnv PRIVATE
    engine
    render
)

target_link_libraries(RLEnv PRIVATE raylib)
