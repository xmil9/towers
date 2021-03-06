//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"


constexpr char AatModel[] = "Armored Assault Tank";
constexpr char AatTexture[] = "aat";
constexpr char AatHitTexture[] = "aat_hit";
constexpr sp::MapDim AatSize{.8f, .8f};
constexpr int AatHp = 4000;
constexpr float AatSpeed = .03f;
constexpr int AatDelay = 0;
constexpr int AatReward = 50;

constexpr char MhcModel[] = "Mobile heavy cannon";
constexpr char MhcTexture[] = "mhc";
constexpr char MhcHitTexture[] = "mhc_hit";
constexpr sp::MapDim MhcSize{.5f, .5f};
constexpr int MhcHp = 1500;
constexpr float MhcSpeed = .05f;
constexpr int MhcDelay = 0;
constexpr int MhcReward = 20;
