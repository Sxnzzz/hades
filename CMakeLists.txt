cmake_minimum_required(VERSION 3.20)

project(hades LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

pkg_check_modules(WLROOTS REQUIRED IMPORTED_TARGET wlroots-0.20)
pkg_check_modules(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)

add_executable(hades
src/main.cpp
src/wlroots_backend.c
)

target_compile_definitions(hades PRIVATE
WLR_USE_UNSTABLE
)

target_link_libraries(hades PRIVATE
PkgConfig::WLROOTS
PkgConfig::WAYLAND_SERVER
)
