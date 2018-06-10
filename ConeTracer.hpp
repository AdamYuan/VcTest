//
// Created by adamyuan on 6/10/18.
//

#ifndef VCTEST_CONETRACER_HPP
#define VCTEST_CONETRACER_HPP

#include <mygl3/texture.hpp>
#include <mygl3/framebuffer.hpp>
#include <asserts/TraceShader.hpp>
#include "GBuffer.hpp"
#include "Voxelize.hpp"

class ConeTracer
{
private:
	mygl3::Texture2D half_result_;
	mygl3::FrameBuffer fbo_;
	asserts::TraceShader trace_shader_;
public:
	void Initialize();
	void Update(const GBuffer &gbuffer, const Voxelize &voxels);
	const mygl3::Texture2D &Get() const { return half_result_; }
};


#endif //VCTEST_CONETRACER_HPP
