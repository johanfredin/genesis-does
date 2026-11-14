//
// Created by johan on 2026-08-29.
//
#include "Controller.h"

#include "../projects/derelict/src/Log.h"

inline void Controller_init() {
    JOY_init();
    LOG_debug("Controller initialized");
}
