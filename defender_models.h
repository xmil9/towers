//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "spiel/coords.h"

// Laser Turret
constexpr char LtModel[] = "Laser Turret";
constexpr char LtTexture[] = "lt";
constexpr char LtFiring1Texture[] = "lt_firing1";
constexpr char LtFiring2Texture[] = "lt_firing2";
constexpr char LtFiringAnimation[] = "lt_firing";
constexpr sp::MapDim LtSize{1.f, 1.f};
constexpr sp::MapCoord LtRange = 1.5f;
constexpr int LtDamage = 10;
constexpr int LtCost = 40;

// Sonic mortar
constexpr char SmModel[] = "Somic Mortar";
constexpr char SmTexture[] = "sm";
constexpr char SmFiring1Texture[] = "sm_firing1";
constexpr char SmFiring2Texture[] = "sm_firing2";
constexpr char SmFiringAnimation[] = "sm_firing";
constexpr sp::MapDim SmSize{1.f, 1.f};
constexpr sp::MapCoord SmRange = 3.f;
constexpr int SmDamage = 6;
constexpr sp::MapCoord SmCollateralRange = 2.f;
constexpr int SmCollateralDamage = 4;
constexpr int SmCost = 50;
