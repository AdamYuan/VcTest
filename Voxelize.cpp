//
// Created by adamyuan on 5/19/18.
//

#include "Voxelize.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Config.hpp"
#include "Resources.hpp"

#include <GLFW/glfw3.h>

void Voxelize::Initialize()
{
	voxel_texture_.Initialize();

	auto *data = new GLubyte[kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4];
	std::fill(data, data + kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4, 0);
	voxel_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
										 GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data), true);
	delete[] data;

	GLfloat border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTextureParameterfv(voxel_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	voxel_texture_.SetSizeFilter(GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST);
	voxel_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);
	glBindImageTexture(7, voxel_texture_.Get(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	shader_.Initialize();
	shader_.SetULightDir(kLightDir);
	shader_.SetULightMatrix(res::light_matrix);

	shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);
	shader_.SetUVoxelWorldSize(kVoxelWorldSize);
}

void Voxelize::Update(const mygl3::Texture2D &shadow_map)
{
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1000, 1000);

	double start = glfwGetTime();
	shader_.Use();
	shadow_map.Bind(5);
	res::sponza_model.Render();
	printf("Voxelize lasted: %lf sec\n", glfwGetTime() - start);

	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
	start = glfwGetTime();
	voxel_texture_.GenerateMipmap();
	printf("Mipmapping lasted: %lf sec\n", glfwGetTime() - start);
}
