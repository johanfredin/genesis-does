cmake_minimum_required(VERSION 4.2)
project(genesis_does C)

set(CMAKE_C_STANDARD 23)

include_directories(
        /opt/toolchains/mars/m68k-elf/inc
        inc
)

add_executable(
        genesis_does

        # common src files will end up here
        inc/Controller.h
        src/Controller.c

        # Projects
        projects/hello-megadrive/src/main.c
        projects/hello-bg/src/main.c
        projects/hello-dpad-callback/src/main.c
        projects/hello-sprite/src/main.c
        projects/hello-animation/src/main.c
        projects/hello-camera/src/main.c
        projects/hello-map/src/main.c
        projects/hello-object-collision/src/main.c
        projects/hello-tiled/src/main.c
        projects/hello-tilebased-collision/src/Grid.h
        projects/hello-tilebased-collision/src/Grid.c


        projects/derelict/src/Entity.c
        projects/derelict/src/Entity.h
        projects/derelict/src/Globals.h
        projects/derelict/src/Log.h
        projects/derelict/src/Game.c
        projects/derelict/src/Game.h
        projects/derelict/src/MainMenu.c
        projects/derelict/src/MainMenu.h
        projects/derelict/src/World.c
        projects/derelict/src/World.h
        projects/derelict/src/Level.c
        projects/derelict/src/Level.h
        projects/derelict/src/LevelDefs.c
        projects/derelict/src/LevelDefs.h

)
