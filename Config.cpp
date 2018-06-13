//
// Created by adamyuan on 5/31/18.
//
#include "Config.hpp"

const glm::vec3 kVoxelGridRangeMin{-32.0f, -6.4f, -16.0f}, kVoxelGridRangeMax{32.0f, 25.6f, 16.0f};
const glm::ivec3 kVoxelDimension{(kVoxelGridRangeMax - kVoxelGridRangeMin) / kVoxelWorldSize};

const glm::vec3 kInitialPos{0, 0, 0};

