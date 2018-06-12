//
// Created by adamyuan on 4/18/18.
//

#ifndef SCENETEST_CONFIG_HPP
#define SCENETEST_CONFIG_HPP

#include <glm/glm.hpp>

constexpr int kWidth = 1440, kHeight = 900;
constexpr int kHalfWidth = kWidth / 2, kHalfHeight = kHeight / 2;
constexpr double kMouseX = kWidth / 2.0, kMouseY = kHeight / 2.0;

constexpr unsigned kInvocationSize = 2;

constexpr int kShadowWidth = 2048, kShadowHeight = 2048;

constexpr float kVoxelWorldSize = 0.125f;
extern const glm::vec3 kVoxelGridRangeMin, kVoxelGridRangeMax;
extern const glm::ivec3 kVoxelDimension;

constexpr float kSpeed = 5.0f, kScale = 50.0f;
extern const glm::vec3 kLightDir;
extern const glm::vec3 kInitialPos;

#endif //SCENETEST_CONFIG_HPP
