#pragma once

#include "controller.hpp"

// Andersson LSD 3.0 IR LED Controller
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
enum class AnderssonButtons {
  Power       = 0xffe01f,
  Auto        = 0xffd02f,
  Randomize   = 0xfff00f,
  Flash       = 0xffe817,
  Jump        = 0xffd827,
  Meteor      = 0xffc837,
  Www         = 0xff609f,
  Inwards     = 0xff50af,
  Outwards    = 0xff10ef,
  Up          = 0xffa05f,
  Down        = 0xff20df,
  ICSet       = 0xff906f,
  CS          = 0xffb04f,
  C3          = 0xffa857,
  C7          = 0xff9867,
  C16         = 0xff8877,
  Faster      = 0xff708f,
  Slower      = 0xff30cf,
  Brighter    = 0xff6897,
  Dimmer      = 0xff28d7,
  Right       = 0xff58a7,
  Left        = 0xff18e7,
  MeteorLeft  = 0xff08f7,
  MeteorRight = 0xff48b7,
};

// LivarnoLux IR LED Controller
static constexpr Controller LivarnoLux{
    .name = "Livarno Lux",
    .bits = 32,
    .eps  = 30,
    .aeps = 100,

    .header = {9500, 4400},
    .one    = {674, 1500},
    .zero   = {674, 440},

    .repeat = {9500, 2126},

    .gap = 108500,
};
enum class LivarnoLuxButtons {
  On       = 0xffb04f,
  Off      = 0xfff807,
  Brighter = 0xff906f,
  Dimmer   = 0xffb847,
  Flash    = 0xffb24d,
  Strobe   = 0xff00ff,
  Fade     = 0xff58a7,
  Smooth   = 0xff30cf,
  White    = 0xffa857,
  Red      = 0xff9867,
  Red1     = 0xffe817,
  Red2     = 0xff02fd,
  Red3     = 0xff50af,
  Red4     = 0xff38c7,
  Green    = 0xffd827,
  Green1   = 0xff48b7,
  Green2   = 0xff32cd,
  Green3   = 0xff7887,
  Green4   = 0xff28d7,
  Blue     = 0xff8877,
  Blue1    = 0xff6897,
  Blue2    = 0xff20df,
  Blue3    = 0xff708f,
  Blue4    = 0xfff00f,
};
