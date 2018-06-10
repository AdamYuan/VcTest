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
#include "ConeTracer.hpp"
#include <asserts/FinalShader.hpp>

class Renderer
{
private:
	Voxelize voxelize_;
	ShadowMap shadow_map_;
	Skybox skybox_;
	GBuffer gbuffer_;
	ConeTracer cone_tracer_;

	asserts::FinalShader final_shader_;

public:
	void Initialize();

	void Render(bool debug_voxel, bool indirect_trace, bool show_albedo, bool show_edge);
};


#endif //SCENETEST_RENDERER_HPP
