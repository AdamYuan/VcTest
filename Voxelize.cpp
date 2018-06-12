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
	//calculate mipmap sizes
	max_mipmap_level_ = mygl3::Texture3D::GetLevel3D(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z);
	printf("Mipmap level: %d\n", max_mipmap_level_);
	glm::uvec3 s{kVoxelDimension};
	for(int level = 0; level < max_mipmap_level_; ++level)
	{
		printf("Mipmap %d: (%d, %d, %d)\n", level, s.x, s.y, s.z);
		mipmap_sizes_.push_back(s);
		glm::uvec3 group_size = s / kInvocationSize;
		if(s.x % kInvocationSize) group_size.x ++;
		if(s.y % kInvocationSize) group_size.y ++;
		if(s.z % kInvocationSize) group_size.z ++;
		compute_group_sizes_.push_back(group_size);

		s = glm::max(s / 2u, glm::uvec3(1));
	}

	initialize_textures();
	initialize_debug_object();

	debug_shader_.Initialize();
	debug_shader_.SetUProjection(res::cam_projection);

	voxelize_shader_.Initialize();
	voxelize_shader_.SetULightDir(kLightDir);
	voxelize_shader_.SetULightMatrix(res::light_matrix);

	voxelize_shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	voxelize_shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);
	voxelize_shader_.SetUVoxelWorldSize(kVoxelWorldSize);

	mipmap_shader_.Initialize();

	bounce_shader_.Initialize();
	bounce_shader_.SetUVoxelDimension(kVoxelDimension);
}

void Voxelize::Update(const mygl3::Texture2D &shadow_map)
{
	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST);
	glBindImageTexture(5, albedo_texture_.Get(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(6, normal_texture_.Get(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(7, radiance_texture_.Get(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1000, 1000);
	double start = glfwGetTime();
	voxelize_shader_.Use();
	shadow_map.Bind(1);
	res::sponza_model.Render();
	printf("Voxelize lasted: %lf sec\n", glfwGetTime() - start);
	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
}

void Voxelize::GenerateMipmap()
{
	double start = glfwGetTime();

	mipmap_shader_.Use();
	for(int level = 1; level < max_mipmap_level_; ++level)
	{
		for(GLuint f = 0; f < 6; ++f)
			glBindImageTexture(f, mipmaps_[f].Get(), level - 1, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16);

		if(level == 1)
			radiance_texture_.Bind(0);
		else
		{
			for(GLuint f = 0; f < 6; ++f)
				mipmaps_[f].Bind(f);
		}

		mipmap_shader_.SetUSourceLod(level - 1);
		mipmap_shader_.SetUVoxelDimensionMipmap(mipmap_sizes_[level]);

		glDispatchCompute(compute_group_sizes_[level].x, compute_group_sizes_[level].y, compute_group_sizes_[level].z);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	//voxel_texture_.GenerateMipmap();
	printf("Mipmapping lasted: %lf sec\n", glfwGetTime() - start);
}

void Voxelize::Bounce()
{
	double start = glfwGetTime();

	bounce_shader_.Use();

	glBindImageTexture(0, radiance_texture_.Get(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16);
	albedo_texture_.Bind(0);
	normal_texture_.Bind(1);
	for(GLuint f = 0; f < 6; ++f) mipmaps_[f].Bind(f + 2);

	glDispatchCompute(compute_group_sizes_[0].x, compute_group_sizes_[0].y, compute_group_sizes_[0].z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	printf("Calculate a diffuse bounce lasted: %lf sec\n", glfwGetTime() - start);
}

void Voxelize::initialize_textures()
{
	auto *data = new GLubyte[kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4];
	std::fill(data, data + kVoxelDimension.x * kVoxelDimension.y * kVoxelDimension.z * 4, 0);
	GLfloat border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//albedo
	albedo_texture_.Initialize();
	albedo_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
										  GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data), false);
	glTextureParameterfv(albedo_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	albedo_texture_.SetSizeFilter(GL_NEAREST, GL_NEAREST);
	albedo_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);

	//normal
	normal_texture_.Initialize();
	normal_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
										  GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, data), false);
	glTextureParameterfv(normal_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	normal_texture_.SetSizeFilter(GL_NEAREST, GL_NEAREST);
	normal_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);

	//radiance
	radiance_texture_.Initialize();
	radiance_texture_.Load(mygl3::ImageInfo(kVoxelDimension.x, kVoxelDimension.y, kVoxelDimension.z,
											GL_RGBA16, GL_RGBA, GL_UNSIGNED_BYTE, data), false);
	glTextureParameterfv(radiance_texture_.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
	radiance_texture_.SetSizeFilter(GL_LINEAR, GL_LINEAR);
	radiance_texture_.SetWrapFilter(GL_CLAMP_TO_BORDER);

	delete[] data;

	//mipmap voxel textures
	for(auto &t : mipmaps_)
	{
		t.Initialize();
		t.Load(mygl3::ImageInfo(mipmap_sizes_[1].x, mipmap_sizes_[1].y, mipmap_sizes_[1].z,
								GL_RGBA16, GL_RGBA, GL_UNSIGNED_BYTE, nullptr), true);
		glTextureParameterfv(t.Get(), GL_TEXTURE_BORDER_COLOR, border_color);
		t.SetSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		t.SetWrapFilter(GL_CLAMP_TO_BORDER);
	}
}

void Voxelize::initialize_debug_object()
{
	std::vector<glm::vec3> vertices;
	std::vector<unsigned> indices;
	glm::vec3 v1, ft1, bt1, v2, ft2, bt2, v3, ft3, bt3, v4, ft4, bt4;
	unsigned index{0};

	//process x plane
	v1.y = v2.y = kVoxelGridRangeMin.y;
	v3.y = v4.y = kVoxelGridRangeMax.y;
	v1.z = v3.z = kVoxelGridRangeMin.z;
	v2.z = v4.z = kVoxelGridRangeMax.z;

	ft1.y = ft2.y = bt1.y = bt2.y = 0.0f;
	ft3.y = ft4.y = bt3.y = bt4.y = 1.0f;
	ft1.z = ft3.z = bt1.z = bt3.z = 0.0f;
	ft2.z = ft4.z = bt2.z = bt4.z = 1.0f;

	for(int i = 0; i < kVoxelDimension.x; ++i)
	{
		v1.x = v2.x = v3.x = v4.x = kVoxelGridRangeMin.x + kVoxelWorldSize * (float)i;
		ft1.x = ft2.x = ft3.x = ft4.x = (i + 0.5f) / (float)kVoxelDimension.x;
		bt1.x = bt2.x = bt3.x = bt4.x = (i - 0.5f) / (float)kVoxelDimension.x;

		vertices.push_back(v1); vertices.push_back(ft1); vertices.push_back(bt1);
		vertices.push_back(v2); vertices.push_back(ft2); vertices.push_back(bt2);
		vertices.push_back(v3); vertices.push_back(ft3); vertices.push_back(bt3);
		vertices.push_back(v4); vertices.push_back(ft4); vertices.push_back(bt4);

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index + 2);
		indices.push_back(index + 1);

		index += 4;
	}

	//process y plane
	v1.x = v2.x = kVoxelGridRangeMin.x;
	v3.x = v4.x = kVoxelGridRangeMax.x;
	v1.z = v3.z = kVoxelGridRangeMin.z;
	v2.z = v4.z = kVoxelGridRangeMax.z;

	ft1.x = ft2.x = bt1.x = bt2.x = 0.0f;
	ft3.x = ft4.x = bt3.x = bt4.x = 1.0f;
	ft1.z = ft3.z = bt1.z = bt3.z = 0.0f;
	ft2.z = ft4.z = bt2.z = bt4.z = 1.0f;

	for(int i = 0; i < kVoxelDimension.y; ++i)
	{
		v1.y = v2.y = v3.y = v4.y = kVoxelGridRangeMin.y + kVoxelWorldSize * (float)i;
		ft1.y = ft2.y = ft3.y = ft4.y = (i - 0.5f) / (float)kVoxelDimension.y;
		bt1.y = bt2.y = bt3.y = bt4.y = (i + 0.5f) / (float)kVoxelDimension.y;

		vertices.push_back(v1); vertices.push_back(ft1); vertices.push_back(bt1);
		vertices.push_back(v2); vertices.push_back(ft2); vertices.push_back(bt2);
		vertices.push_back(v3); vertices.push_back(ft3); vertices.push_back(bt3);
		vertices.push_back(v4); vertices.push_back(ft4); vertices.push_back(bt4);

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index + 2);
		indices.push_back(index + 1);

		index += 4;
	}

	//process z plane
	v1.x = v2.x = kVoxelGridRangeMin.x;
	v3.x = v4.x = kVoxelGridRangeMax.x;
	v1.y = v3.y = kVoxelGridRangeMin.y;
	v2.y = v4.y = kVoxelGridRangeMax.y;

	ft1.x = ft2.x = bt1.x = bt2.x = 0.0f;
	ft3.x = ft4.x = bt3.x = bt4.x = 1.0f;
	ft1.y = ft3.y = bt1.y = bt3.y = 0.0f;
	ft2.y = ft4.y = bt2.y = bt4.y = 1.0f;

	for(int i = 0; i < kVoxelDimension.z; ++i)
	{
		v1.z = v2.z = v3.z = v4.z = kVoxelGridRangeMin.z + kVoxelWorldSize * (float)i;
		ft1.z = ft2.z = ft3.z = ft4.z = (i + 0.5f) / (float)kVoxelDimension.z;
		bt1.z = bt2.z = bt3.z = bt4.z = (i - 0.5f) / (float)kVoxelDimension.z;

		vertices.push_back(v1); vertices.push_back(ft1); vertices.push_back(bt1);
		vertices.push_back(v2); vertices.push_back(ft2); vertices.push_back(bt2);
		vertices.push_back(v3); vertices.push_back(ft3); vertices.push_back(bt3);
		vertices.push_back(v4); vertices.push_back(ft4); vertices.push_back(bt4);

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index + 2);
		indices.push_back(index + 1);

		index += 4;
	}

	debug_object_.Initialize();
	debug_object_.SetData(vertices, GL_STATIC_DRAW);
	debug_object_.SetIndices(indices, GL_STATIC_DRAW);
	debug_object_.SetAttributes(0, 3, 1, 3, 2, 3);
}

void Voxelize::Debug()
{
	glDisable(GL_CULL_FACE);
	radiance_texture_.SetSizeFilter(GL_NEAREST, GL_NEAREST);
	radiance_texture_.Bind(0);
	debug_shader_.Use();
	debug_shader_.SetUView(res::cam_view);

	debug_object_.Render(GL_TRIANGLES);
	radiance_texture_.SetSizeFilter(GL_LINEAR, GL_LINEAR);
	glEnable(GL_CULL_FACE);
}
