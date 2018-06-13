//
// Created by adamyuan on 6/10/18.
//

#include "ConeTracer.hpp"
#include "Config.hpp"
#include "Resources.hpp"
#include "Voxels.hpp"

void ConeTracer::Initialize()
{
	half_result_.Initialize();
	half_result_.Load(mygl3::ImageInfo(kHalfWidth, kHalfHeight, 0, GL_RGB16, 0, 0, nullptr), false);
	half_result_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	half_result_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	fbo_.Initialize();
	fbo_.AttachTexture(half_result_, GL_COLOR_ATTACHMENT0);

	trace_shader_.Initialize();

	trace_shader_.SetUProjection(res::cam_projection);

	trace_shader_.SetUVoxelDimension(kVoxelDimension);
	trace_shader_.SetUVoxelWorldSize(kVoxelWorldSize);
	trace_shader_.SetUVoxelGridRangeMin(kVoxelGridRangeMin);
	trace_shader_.SetUVoxelGridRangeMax(kVoxelGridRangeMax);
}

void ConeTracer::Update(const GBuffer &gbuffer, const Voxels &voxels)
{
	mygl3::FrameBufferBinder binder(fbo_);

	glViewport(0, 0, kHalfWidth, kHalfHeight);

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT);
	gbuffer.GetHalfPositionTexture().Bind(0);
	gbuffer.GetHalfNormalTexture().Bind(1);
	voxels.GetRadiance().Bind(6);
	for(int i = 0; i < 6; ++i) voxels.GetMipmap()[i].Bind(i + 7u);

	trace_shader_.Use();
	trace_shader_.SetUView(res::cam_view);
	trace_shader_.SetUCamPosition(res::cam_pos);

	res::quad_object.Render(GL_TRIANGLES);
}
