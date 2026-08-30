cmake_minimum_required(VERSION 4.2)
project(genesis_does C)

set(CMAKE_C_STANDARD 11)

include_directories(
        /opt/toolchains/mars/m68k-elf/inc
        inc
)

add_executable(
        genesis_does

        # common src files will end up here
        src/Controller.c

        # Projects
        projects/hello-megadrive/src/main.c
        projects/hello-bg/src/main.c
        projects/hello-dpad-callback/src/main.c
        projects/tmx-map/src/main.c
        projects/hello-sprite/src/main.c
        projects/hello-animation/src/main.c
        inc/Controller.h
        src/Controller.c
        projects/hello-controller/src/main.c
)
