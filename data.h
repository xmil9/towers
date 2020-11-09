// Nov-2020, Michael Lindner
// MIT license
//
#pragma once
#include "gll_data_format.h"


static float positions[] = {
   // Front face
   // lt
   -0.6f,
   0.6f,
   0.0f,
   // rt
   0.6f,
   0.6f,
   0.0f,
   // rb
   0.6f,
   -0.6f,
   0.0f,
   // lb
   -0.6f,
   -0.6f,
   0.0f,
   // Left face
   // lt
   -0.6f,
   0.6f,
   -1.2f,
   // rt
   -0.6f,
   0.6f,
   0.0f,
   // rb
   -0.6f,
   -0.6f,
   0.0f,
   // lb
   -0.6f,
   -0.6f,
   -1.2f,
   // Top face
   // lt
   -0.6f,
   0.6f,
   -1.2f,
   // rt
   0.6f,
   0.6f,
   -1.2f,
   // rb
   0.6f,
   0.6f,
   0.0f,
   // lb
   -0.6f,
   0.6f,
   0.0f,
   // Right face
   // lt
   0.6f,
   0.6f,
   0.0f,
   // rt
   0.6f,
   0.6f,
   -1.2f,
   // rb
   0.6f,
   -0.6f,
   -1.2f,
   // lb
   0.6f,
   -0.6f,
   0.0f,
   // Bottom face
   // lt
   0.6f,
   -0.6f,
   -1.2f,
   // rt
   -0.6f,
   -0.6f,
   -1.2f,
   // rb
   -0.6f,
   -0.6f,
   0.0f,
   // lb
   0.6f,
   -0.6f,
   0.0f,
   // Back face
   // lt
   0.6f,
   0.6f,
   -1.2f,
   // rt
   -0.6f,
   0.6f,
   -1.2f,
   // rb
   -0.6f,
   -0.6f,
   -1.2f,
   // lb
   0.6f,
   -0.6f,
   -1.2f,
};
static constexpr gll::DataFormat posFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                              nullptr};

static float normals[] = {
   // Front face
   // lt
   0.0f,
   0.0f,
   1.0f,
   // rt
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   0.0f,
   1.0f,
   // Left face
   // lt
   -1.0f,
   0.0f,
   0.0f,
   // rt
   -1.0f,
   0.0f,
   0.0f,
   // rb
   -1.0f,
   0.0f,
   0.0f,
   // lb
   -1.0f,
   0.0f,
   0.0f,
   // Top face
   // lt
   0.0f,
   1.0f,
   0.0f,
   // rt
   0.0f,
   1.0f,
   0.0f,
   // rb
   0.0f,
   1.0f,
   0.0f,
   // lb
   0.0f,
   1.0f,
   0.0f,
   // Right face
   // lt
   1.0f,
   0.0f,
   0.0f,
   // rt
   1.0f,
   0.0f,
   0.0f,
   // rb
   1.0f,
   0.0f,
   0.0f,
   // lb
   1.0f,
   0.0f,
   0.0f,
   // Bottom face
   // lt
   0.0f,
   -1.0f,
   0.0f,
   // rt
   0.0f,
   -1.0f,
   0.0f,
   // rb
   0.0f,
   -1.0f,
   0.0f,
   // lb
   0.0f,
   -1.0f,
   0.0f,
   // Back face
   // lt
   0.0f,
   0.0f,
   -1.0f,
   // rt
   0.0f,
   0.0f,
   -1.0f,
   // rb
   0.0f,
   0.0f,
   -1.0f,
   // lb
   0.0f,
   0.0f,
   -1.0f,
};
static constexpr gll::DataFormat normalFormat = {3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                                 nullptr};

static unsigned int indices[] = {
   // Front face
   // Triangle 1
   0,
   1,
   2,
   // Triangle 2
   2,
   3,
   0,
   // Left face
   // Triangle 1
   4,
   5,
   6,
   // Triangle 2
   6,
   7,
   4,
   // Top face
   // Triangle 1
   8,
   9,
   10,
   // Triangle 2
   10,
   11,
   8,
   // Right face
   // Triangle 1
   12,
   13,
   14,
   // Triangle 2
   14,
   15,
   12,
   // Bottom face
   // Triangle 1
   16,
   17,
   18,
   // Triangle 2
   18,
   19,
   16,
   // Back face
   // Triangle 1
   20,
   21,
   22,
   // Triangle 2
   22,
   23,
   20,
};

static float colors[] = {
   // Front face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Left face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Top face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Right face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Bottom face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
   // Back face
   // lt
   0.0f,
   0.6f,
   0.0f,
   1.0f,
   // rt
   -0.6f,
   0.0f,
   0.0f,
   1.0f,
   // rb
   0.6f,
   0.0f,
   0.0f,
   1.0f,
   // lb
   0.0f,
   -0.6f,
   0.0f,
   1.0f,
};
static constexpr gll::DataFormat colorFormat = {4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                                                nullptr};

// Texture coord system:
// (0, 0) left-bottom
// (1, 1) right-top
static float texCoords[] = {
   // Front face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Left face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Top face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Right face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Bottom face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
   // Back face
   // lt
   0.0f,
   1.0f,
   // rt
   1.0f,
   1.0f,
   // rb
   1.0f,
   0.0f,
   // lb
   0.0f,
   0.0f,
};
static constexpr gll::DataFormat texCoordFormat = {2, GL_FLOAT, GL_FALSE,
                                                   2 * sizeof(float), nullptr};
