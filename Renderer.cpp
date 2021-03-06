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

	voxels_.Initialize();

	voxels_.Voxelize();
	voxels_.DirectLight(shadow_map_.Get());
	voxels_.GenerateMipmap();

	voxels_.Bounce();
	voxels_.GenerateMipmap();

	gbuffer_.Initialize();

	cone_tracer_.Initialize();


	final_shader_.Initialize();

	final_shader_.SetUProjection(res::cam_projection);

	final_shader_.SetUVoxelDimension(kVoxelDimension);
	final_shader_.SetUVoxelWorldSize(kVoxelWorldSize);
	final_shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	final_shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);

	final_shader_.SetUResolution(glm::ivec2(kWidth, kHeight));
}

void Renderer::Render(bool debug_voxel, bool indirect_trace, bool show_albedo, bool show_edge)
{
	if(debug_voxel)
	{
		glViewport(0, 0, kWidth, kHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		voxels_.Debug();
		return;
	}

	gbuffer_.Update();

	if(indirect_trace)
		cone_tracer_.Update(gbuffer_, voxels_);

	glViewport(0, 0, kWidth, kHeight);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);
	gbuffer_.GetPositionTexture().Bind(0);
	gbuffer_.GetNormalTexture().Bind(1);
	gbuffer_.GetAlbedoTexture().Bind(2);
	cone_tracer_.Get().Bind(3);
	skybox_.Get().Bind(4);
	shadow_map_.Get().Bind(5);
	voxels_.GetRadiance().Bind(6);
	for(int i = 0; i < 6; ++i) voxels_.GetMipmap()[i].Bind(i + 9u);

	final_shader_.Use();
	final_shader_.SetULightDir(res::light_dir);
	final_shader_.SetULightMatrix(res::light_matrix);

	final_shader_.SetUView(res::cam_view);
	final_shader_.SetUCamPosition(res::cam_pos);
	//set flags
	final_shader_.SetUEnableIndirectTrace(indirect_trace);
	final_shader_.SetUShowAlbedo(show_albedo);
	final_shader_.SetUShowEdge(show_edge);
	res::quad_object.Render(GL_TRIANGLES);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::UpdateLight()
{
	shadow_map_.Update();

	voxels_.DirectLight(shadow_map_.Get());
	voxels_.GenerateMipmap();
	voxels_.Bounce();
	voxels_.GenerateMipmap();
}

