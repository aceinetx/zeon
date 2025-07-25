cmake_minimum_required(VERSION 3.20)
project(zeon)

include("cmake/config.cmake")

add_library(imgui STATIC ${IMGUI_SOURCES} "imgui/backends/imgui_impl_sdl3.cpp" "imgui/backends/imgui_impl_sdlrenderer3.cpp" "imgui/imgui.cpp" "imgui/imgui_demo.cpp" "imgui/imgui_draw.cpp" "imgui/imgui_tables.cpp" "imgui/imgui_widgets.cpp")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cc")
add_executable(zeon ${SOURCES})
add_dependencies(zeon copy_assets)

target_link_libraries(zeon imgui SDL3::SDL3 libcef_dll_wrapper cef)
