//
// Created by adamyuan on 4/18/18.
//

#include <unordered_map>
#include "Renderer.hpp"
#include "Config.hpp"

#include "Resources.hpp"

#include <GLFW/glfw3.h>

void Renderer::Initialize()
{
	shadow_map_.Initialize();
	shadow_map_.Update();

	skybox_.Initialize();

	voxelize_.Initialize();
	voxelize_.Update(shadow_map_.Get());

	gbuffer_.Initialize();

	trace_shader_.Initialize();
	trace_shader_.LoadFromFile("shaders/trace.frag", GL_FRAGMENT_SHADER);
	trace_shader_.LoadFromFile("shaders/trace.vert", GL_VERTEX_SHADER);
	//camera
	trace_unif_projection_ = trace_shader_.GetUniform("uProjection");
	trace_unif_view_ = trace_shader_.GetUniform("uView");
	trace_unif_cam_position_ = trace_shader_.GetUniform("uCamPosition");
	//light
	trace_unif_light_matrix_ = trace_shader_.GetUniform("uLightMatrix");
	trace_unif_light_dir_ = trace_shader_.GetUniform("uLightDir");
	//voxel settings
	trace_unif_voxel_dimension_ = trace_shader_.GetUniform("uVoxelDimension");
	trace_unif_voxel_world_size_ = trace_shader_.GetUniform("uVoxelWorldSize");
	trace_unif_voxel_grid_range_min_ = trace_shader_.GetUniform("uVoxelGridRangeMin");
	trace_unif_voxel_grid_range_max_ = trace_shader_.GetUniform("uVoxelGridRangeMax");
	//flags
	trace_unif_indirect_trace_ = trace_shader_.GetUniform("uEnableIndirectTrace");
	trace_unif_show_albedo_ = trace_shader_.GetUniform("uShowAlbedo");
	trace_unif_debug_voxel_ = trace_shader_.GetUniform("uDebugVoxel");

	trace_shader_.SetMat4(trace_unif_projection_, res::cam_projection);
	trace_shader_.SetMat4(trace_unif_light_matrix_, res::light_matrix);
	trace_shader_.SetVec3(trace_unif_light_dir_, kLightDir);

	trace_shader_.SetIVec3(trace_unif_voxel_dimension_, kVoxelDimension);
	trace_shader_.SetFloat(trace_unif_voxel_world_size_, kVoxelWorldSize);
	trace_shader_.SetVec3(trace_unif_voxel_grid_range_min_, kVoxelGridRangeMin);
	trace_shader_.SetVec3(trace_unif_voxel_grid_range_max_, kVoxelGridRangeMax);
}

void Renderer::Render(bool debug_voxel, bool indirect_trace, bool show_albedo)
{
	glViewport(0, 0, kWidth, kHeight);

	gbuffer_.Update();

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);
	gbuffer_.GetPositionTexture().Bind(GL_TEXTURE0);
	gbuffer_.GetNormalTexture().Bind(GL_TEXTURE1);
	gbuffer_.GetAlbedoTexture().Bind(GL_TEXTURE2);
	skybox_.Get().Bind(GL_TEXTURE4);
	shadow_map_.Get().Bind(GL_TEXTURE5);
	voxelize_.Get().Bind(GL_TEXTURE6);

	trace_shader_.Use();

	trace_shader_.SetMat4(trace_unif_view_, res::cam_view);
	trace_shader_.SetVec3(trace_unif_cam_position_, res::cam_pos);
	//set flags
	trace_shader_.SetInt(trace_unif_indirect_trace_, indirect_trace);
	trace_shader_.SetInt(trace_unif_show_albedo_, show_albedo);
	trace_shader_.SetInt(trace_unif_debug_voxel_, debug_voxel);
	res::quad_object.Render(GL_TRIANGLES);

	glEnable(GL_DEPTH_TEST);
}

