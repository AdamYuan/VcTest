//
// Created by adamyuan on 5/19/18.
//

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Voxelize.hpp"
#include "Config.hpp"
#include "Resources.hpp"
#include <GLFW/glfw3.h>

void Voxelize::Initialize()
{
	auto *data = new GLubyte[kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4];
	std::fill(data, data + kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4, 0);
	/*for(int i = 0; i < kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z; ++i)
	{
		data[i*4 + 3] = 255;
	}*/
	voxel_texture_.Initialize();
	voxel_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
										 GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data), true);

	//printf("%d\n", mygl3::Texture3D::GetLevel3D(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z));
	delete[] data;
	GLfloat border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTextureParameterfv(voxel_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	voxel_texture_.SetSizeFilter(GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST);
	voxel_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);
	glBindImageTexture(7, voxel_texture_.Get(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

	shader_.Initialize();
	shader_.LoadFromFile("shaders/voxelize.vert", GL_VERTEX_SHADER);
	shader_.LoadFromFile("shaders/voxelize.frag", GL_FRAGMENT_SHADER);
	shader_.LoadFromFile("shaders/voxelize.geom", GL_GEOMETRY_SHADER);
	unif_light_matrix_ = shader_.GetUniform("uLightMatrix");
	unif_light_dir_ = shader_.GetUniform("uLightDir");
	unif_voxel_world_size_ = shader_.GetUniform("uVoxelWorldSize");
	unif_voxel_grid_range_min_ = shader_.GetUniform("uVoxelGridRangeMin");
	unif_voxel_grid_range_max_ = shader_.GetUniform("uVoxelGridRangeMax");
	shader_.SetMat4(unif_light_matrix_, res::light_matrix);
	shader_.SetVec3(unif_light_dir_, kLightDir);
	shader_.SetFloat(unif_voxel_world_size_, kVoxelWorldSize);
	shader_.SetVec3(unif_voxel_grid_range_min_, kVoxelGridRangeMin);
	shader_.SetVec3(unif_voxel_grid_range_max_, kVoxelGridRangeMax);
}

void Voxelize::Update(const mygl3::Texture2D &shadow_map)
{
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1000, 1000);

	shader_.Use();
	shadow_map.Bind(GL_TEXTURE5);
	res::sponza_model.Render();

	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
	double start = glfwGetTime();
	voxel_texture_.GenerateMipmap();
	printf("Voxelize lasted: %lf sec\n", glfwGetTime() - start);
}
