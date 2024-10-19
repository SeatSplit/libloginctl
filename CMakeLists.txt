cmake_minimum_required(VERSION 3.29)
project(libloginctl C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SYSTEMD REQUIRED libsystemd)
pkg_check_modules(CHECK REQUIRED check)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${SYSTEMD_INCLUDE_DIRS})
include_directories(${CHECK_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})

add_library(libloginctl STATIC source/loginctl.c
        include/loginctl.h)

target_link_libraries(libloginctl ${SYSTEMD_LIBRARIES} ${GLIB_LIBRARIES})

target_include_directories(libloginctl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(libloginctl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

enable_testing()

add_executable(test_loginctl test/test_loginctl.c)

target_link_libraries(test_loginctl libloginctl ${CHECK_LIBRARIES} ${GLIB_LIBRARIES})

add_test(NAME libloginctl_test COMMAND test_loginctl)
