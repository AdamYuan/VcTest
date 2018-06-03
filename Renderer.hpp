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
#include <asserts/TraceShader.hpp>

class Renderer
{
private:
	Voxelize voxelize_;
	ShadowMap shadow_map_;
	Skybox skybox_;
	GBuffer gbuffer_;

	asserts::TraceShader trace_shader_;

public:
	void Initialize();

	void Render(bool debug_voxel, bool indirect_trace, bool show_albedo);
};


#endif //SCENETEST_RENDERER_HPP
