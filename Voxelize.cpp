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
	max_mipmap_level_ = mygl3::Texture3D::GetLevel3D(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z);

	printf("Mipmap level: %d\n", max_mipmap_level_);
	printf("Voxel dimension: (%d, %d, %d)\n", kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z);

	//base voxel texture
	voxel_texture_.Initialize();
	auto *data = new GLubyte[kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4];
	std::fill(data, data + kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4, 0);
	voxel_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
										 GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data), false);
	delete[] data;
	GLfloat border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTextureParameterfv(voxel_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	voxel_texture_.SetSizeFilter(GL_LINEAR, GL_LINEAR);
	voxel_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);

	//mipmap voxel textures
	for(auto &t : mipmap_textures_)
	{
		t.Initialize();
		t.Load(mygl3::ImageInfo(kVoxelDimension.x / 2, kVoxelDimension.y / 2, kVoxelDimension.z / 2,
								GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr), true);
		glTextureParameterfv(t.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
		t.SetSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		t.SetWrapFilter(GL_CLAMP_TO_BORDER);
	}

	voxelize_shader_.Initialize();
	voxelize_shader_.SetULightDir(kLightDir);
	voxelize_shader_.SetULightMatrix(res::light_matrix);

	voxelize_shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	voxelize_shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);
	voxelize_shader_.SetUVoxelWorldSize(kVoxelWorldSize);

	mipmap_shader_.Initialize();
}

void Voxelize::Update(const mygl3::Texture2D &shadow_map)
{
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST);
	glBindImageTexture(7, voxel_texture_.Get(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1000, 1000);
	double start = glfwGetTime();
	voxelize_shader_.Use();
	shadow_map.Bind(5);
	res::sponza_model.Render();
	printf("Voxelize lasted: %lf sec\n", glfwGetTime() - start);
	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);


	//mipmap
	start = glfwGetTime();

	glm::ivec3 mipmap_texture_size{kVoxelDimension};

	mipmap_shader_.Use();
	for(int level = 1; level < max_mipmap_level_; ++level)
	{
		for(GLuint f = 0; f < 6; ++f)
			glBindImageTexture(f, mipmap_textures_[f].Get(), level - 1, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

		if(level == 1)
			voxel_texture_.Bind(0);
		else
		{
			for(GLuint f = 0; f < 6; ++f)
				mipmap_textures_[f].Bind(f);
		}

		mipmap_shader_.SetUSourceLod(level - 1);

		mipmap_texture_size = glm::max(mipmap_texture_size / 2, glm::ivec3(1));
		printf("Voxel %d: (%d, %d, %d)\n", level, mipmap_texture_size.x, mipmap_texture_size.y, mipmap_texture_size.z);

		mipmap_shader_.SetUVoxelDimensionMipmap(mipmap_texture_size);

		glm::uvec3 compute_group{mipmap_texture_size / 4 + 1};
		glDispatchCompute(compute_group.x, compute_group.y, compute_group.z);
	}

	//voxel_texture_.GenerateMipmap();
	printf("Mipmapping lasted: %lf sec\n", glfwGetTime() - start);
}
