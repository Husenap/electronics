#pragma once

#include "controller.hpp"

// Small controller for LED strips
static constexpr Controller Andersson{
    .name = "Andersson",
    .bits = 32,
    .eps  = 30,
    .aeps = 100,

    .header = {9500, 4400},
    .one    = {674, 1500},
    .zero   = {674, 440},

    .repeat = {9500, 2126},

    .gap = 108500,
};
