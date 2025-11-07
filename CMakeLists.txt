cmake_minimum_required(VERSION 3.10)
project(ImageProcessor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    external/imgui
    external/imgui/backends
    src
)

set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_sdl2.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

set(PROJECT_SOURCES
    src/main.cpp
)

add_executable(image_processor
    ${IMGUI_SOURCES}
    ${PROJECT_SOURCES}
)

target_link_libraries(image_processor
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

if (WIN32)
    target_link_libraries(image_processor opengl32 comdlg32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})