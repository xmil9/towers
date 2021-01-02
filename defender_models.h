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
constexpr NormCoord LtSize = .04f;
constexpr NormCoord LtRange = .1f;
constexpr int LtDamage = 5;

// Sonic mortar
constexpr char SmModel[] = "Somic Mortar";
constexpr char SmTexture[] = "sm";
constexpr char SmFiring1Texture[] = "sm_firing1";
constexpr char SmFiring2Texture[] = "sm_firing2";
constexpr char SmFiringAnimation[] = "sm_firing";
constexpr NormCoord SmSize = .04f;
constexpr NormCoord SmRange = .2f;
constexpr int SmDamage = 3;
