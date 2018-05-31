//
// Created by adamyuan on 5/19/18.
//

#ifndef VXGITEST_VOXELIZE_HPP
#define VXGITEST_VOXELIZE_HPP

#include <mygl3/vertexobject.hpp>
#include <mygl3/shader.hpp>
#include <mygl3/texture.hpp>

class Voxelize
{
private:
	mygl3::Texture3D voxel_texture_;
	mygl3::Shader shader_;
	GLint unif_light_matrix_, unif_light_dir_, unif_voxel_grid_range_min_,
			unif_voxel_grid_range_max_, unif_voxel_world_size_;

	//mygl3::VertexObject<false> debug_object_;
	//mygl3::Shader debug_shader_; GLint debug_unif_projection_, debug_unif_view_;
public:
	void Initialize();
	void Update(const mygl3::Texture2D &shadow_map);
	//void Debug();
	const mygl3::Texture3D &Get() const { return voxel_texture_; }
};


#endif //VXGITEST_VOXELIZE_HPP
