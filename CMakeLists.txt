cmake_minimum_required(VERSION 3.10)
project(ImageProcessor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    external/imgui
    external/imgui/backends
    src
)

# Dear ImGui source files
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

# Project source files
set(PROJECT_SOURCES
    src/main.cpp
)

# Create executable
add_executable(image_processor
    ${IMGUI_SOURCES}
    ${PROJECT_SOURCES}
)

# Link libraries
target_link_libraries(image_processor
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

if(UNIX)
    target_link_libraries(image_processor pthread)
endif()

# Copy assets to build directory
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Installation
install(TARGETS image_processor DESTINATION bin)
install(DIRECTORY assets DESTINATION share/image_processor)
