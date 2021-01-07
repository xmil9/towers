//
// Dec-2020, Michael Lindner
// MIT license
//
#pragma once
#include "coords.h"

// Laser Turret
constexpr char LtModel[] = "Laser Turret";
constexpr char LtTexture[] = "lt";
constexpr char LtFiring1Texture[] = "lt_firing1";
constexpr char LtFiring2Texture[] = "lt_firing2";
constexpr char LtFiringAnimation[] = "lt_firing";
constexpr MapCoord LtSize = 1.f;
constexpr MapCoord LtRange = 1.5f;
constexpr int LtDamage = 5;

// Sonic mortar
constexpr char SmModel[] = "Somic Mortar";
constexpr char SmTexture[] = "sm";
constexpr char SmFiring1Texture[] = "sm_firing1";
constexpr char SmFiring2Texture[] = "sm_firing2";
constexpr char SmFiringAnimation[] = "sm_firing";
constexpr MapCoord SmSize = 1.f;
constexpr MapCoord SmRange = 3.f;
constexpr int SmDamage = 3;
constexpr MapCoord SmCollateralRange = 1.f;
constexpr int SmCollateralDamage = 1;
