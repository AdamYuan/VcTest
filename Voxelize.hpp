//
// Created by adamyuan on 5/19/18.
//

#ifndef VXGITEST_VOXELIZE_HPP
#define VXGITEST_VOXELIZE_HPP

#include <mygl3/vertexobject.hpp>
#include <asserts/VoxelizeShader.hpp>
#include <asserts/VoxelMipmapShader.hpp>
#include <mygl3/texture.hpp>

class Voxelize
{
private:
	mygl3::Texture3D voxel_texture_;
	asserts::VoxelizeShader voxelize_shader_;
	asserts::VoxelMipmapShader mipmap_shader_;
	int max_mipmap_level_;
public:
	void Initialize();
	void Update(const mygl3::Texture2D &shadow_map);
	const mygl3::Texture3D &Get() const { return voxel_texture_; }
};


#endif //VXGITEST_VOXELIZE_HPP
