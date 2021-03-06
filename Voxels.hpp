//
// Created by adamyuan on 5/19/18.
//

#ifndef VXGITEST_VOXELS_HPP
#define VXGITEST_VOXELS_HPP

#include <vector>
#include <mygl3/vertexobject.hpp>
#include <mygl3/texture.hpp>
#include <asserts/VoxelizeShader.hpp>
#include <asserts/VoxelMipmapShader.hpp>
#include <asserts/VoxelDebugShader.hpp>
#include <asserts/VoxelBounceShader.hpp>
#include <asserts/VoxelDirectLightShader.hpp>

class Voxels
{
private:
	mygl3::Texture3D radiance_texture_, normal_texture_, albedo_texture_, mipmaps_[6];
	asserts::VoxelizeShader voxelize_shader_;
	asserts::VoxelMipmapShader mipmap_shader_;
	int max_mipmap_level_;
	std::vector<glm::uvec3> mipmap_sizes_, compute_group_sizes_;

	asserts::VoxelDebugShader debug_shader_;
	mygl3::VertexObject<true> debug_object_;

	asserts::VoxelBounceShader bounce_shader_;
	asserts::VoxelDirectLightShader direct_light_shader_;
	void initialize_textures();
	void initialize_debug_object();
public:
	void Initialize();
	void Voxelize();
	void GenerateMipmap();
	void DirectLight(const mygl3::Texture2D &shadow_map);
	void Bounce();
	void Debug();
	const mygl3::Texture3D &GetAlbedo() const { return albedo_texture_; }
	const mygl3::Texture3D &GetNormal() const { return normal_texture_; }
	const mygl3::Texture3D &GetRadiance() const { return radiance_texture_; }
	const decltype(mipmaps_) &GetMipmap() const { return mipmaps_; }
};


#endif //VXGITEST_VOXELS_HPP
