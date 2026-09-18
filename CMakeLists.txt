cmake_minimum_required(VERSION 3.20)

project(hades LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(WLROOTS REQUIRED IMPORTED_TARGET wlroots-0.20)
pkg_check_modules(WAYLAND_SERVER REQUIRED IMPORTED_TARGET wayland-server)

add_executable(hades
src/main.cpp
)

target_link_libraries(hades PRIVATE
PkgConfig::WLROOTS
PkgConfig::WAYLAND_SERVER
)
