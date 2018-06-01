//
// Created by adamyuan on 4/18/18.
//

#ifndef SCENETEST_RENDERER_HPP
#define SCENETEST_RENDERER_HPP

#include <mygl3/framebuffer.hpp>
#include "ShadowMap.hpp"
#include "Skybox.hpp"
#include "Voxelize.hpp"
#include "GBuffer.hpp"

class Renderer
{
private:
	Voxelize voxelize_;
	ShadowMap shadow_map_;
	Skybox skybox_;
	GBuffer gbuffer_;

	//tracing shader
	mygl3::Shader trace_shader_;
	GLint trace_unif_projection_, trace_unif_view_, trace_unif_cam_position_; //camera
	GLint trace_unif_light_dir_, trace_unif_light_matrix_; //light
	GLint trace_unif_voxel_dimension_, trace_unif_voxel_grid_range_min_, trace_unif_voxel_grid_range_max_,
			trace_unif_voxel_world_size_; //voxel settings
	GLint trace_unif_indirect_trace_, trace_unif_show_albedo_, trace_unif_debug_voxel_; //flags

public:
	void Initialize();

	void Render(bool debug_voxel, bool indirect_trace, bool show_albedo);
};


#endif //SCENETEST_RENDERER_HPP
