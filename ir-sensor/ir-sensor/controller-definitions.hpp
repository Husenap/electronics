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
