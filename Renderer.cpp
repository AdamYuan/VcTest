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

	trace_shader_.SetUProjection(res::cam_projection);
	trace_shader_.SetULightMatrix(res::light_matrix);
	trace_shader_.SetULightDir(kLightDir);

	trace_shader_.SetUVoxelDimension(kVoxelDimension);
	trace_shader_.SetUVoxelWorldSize(kVoxelWorldSize);
	trace_shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	trace_shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);
}

void Renderer::Render(bool debug_voxel, bool indirect_trace, bool show_albedo)
{
	glViewport(0, 0, kWidth, kHeight);

	gbuffer_.Update();

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);
	gbuffer_.GetPositionTexture().Bind(0);
	gbuffer_.GetNormalTexture().Bind(1);
	gbuffer_.GetAlbedoTexture().Bind(2);
	skybox_.Get().Bind(4);
	shadow_map_.Get().Bind(5);
	voxelize_.Get().Bind(6);
	for(int i = 0; i < 6; ++i)
		voxelize_.GetMipmap()[i].Bind(i + 7u);

	trace_shader_.Use();

	trace_shader_.SetUView(res::cam_view);
	trace_shader_.SetUCamPosition(res::cam_pos);
	//set flags
	trace_shader_.SetUEnableIndirectTrace(indirect_trace);
	trace_shader_.SetUShowAlbedo(show_albedo);
	trace_shader_.SetUDebugVoxel(debug_voxel);
	res::quad_object.Render(GL_TRIANGLES);

	glEnable(GL_DEPTH_TEST);
}

