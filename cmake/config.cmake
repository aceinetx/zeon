cmake_minimum_required(VERSION 3.20)
include("cmake/DownloadCEF.cmake")

include_directories("imgui" "imgui/backends")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CEF_VERSION "138.0.33+g276ed6d+chromium-138.0.7204.169")
set(CEF_CHANNEL "")

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    if("${PROJECT_ARCH}" STREQUAL "arm64")
        set(CEF_PLATFORM "macosarm64")
    elseif("${PROJECT_ARCH}" STREQUAL "x86_64")
        set(CEF_PLATFORM "macosx64")
    elseif("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "arm64")
        set(PROJECT_ARCH "arm64")
        set(CEF_PLATFORM "macosarm64")
    else()
        set(PROJECT_ARCH "x86_64")
        set(CEF_PLATFORM "macosx64")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm")
        set(CEF_PLATFORM "linuxarm")
    elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
        set(CEF_PLATFORM "linuxarm64")
    elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "linux64")
    else()
        message(FATAL_ERROR "Linux x86 32-bit builds are discontinued.")
    endif()
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    if("${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" STREQUAL "ARM64")
        set(CEF_PLATFORM "windowsarm64")
    elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
        set(CEF_PLATFORM "windows64")
    else()
        set(CEF_PLATFORM "windows32")
    endif()
endif()

DownloadCEF("${CEF_PLATFORM}" "${CEF_CHANNEL}" "${CEF_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/cef")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

find_package(CEF REQUIRED)
find_package(SDL3 REQUIRED)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DESTINATION_DIR "${CMAKE_BINARY_DIR}/assets")

file(COPY ${ASSETS_SOURCE_DIR} DESTINATION ${ASSETS_DESTINATION_DIR})

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets"
    COMMENT "Copying assets from ${ASSETS_SOURCE_DIR} to ${ASSETS_DESTINATION_DIR}"
)
