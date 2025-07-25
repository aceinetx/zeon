cmake_minimum_required(VERSION 3.20)

include_directories("imgui" "imgui/backends")

find_package(Vulkan REQUIRED)
find_package(SDL3 REQUIRED)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DESTINATION_DIR "${CMAKE_BINARY_DIR}/assets")

file(COPY ${ASSETS_SOURCE_DIR} DESTINATION ${ASSETS_DESTINATION_DIR})

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets from ${ASSETS_SOURCE_DIR} to ${ASSETS_DESTINATION_DIR}"
)
