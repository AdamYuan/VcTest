//
// Created by adamyuan on 4/23/18.
//

#ifndef SCENETEST_SHADOWMAP_HPP
#define SCENETEST_SHADOWMAP_HPP

#include <mygl3/framebuffer.hpp>
#include <asserts/ShadowShader.hpp>
#include "Model.hpp"

class ShadowMap
{
private:
	//shadow map
	asserts::ShadowShader shadow_shader_;
	mygl3::FrameBuffer shadow_fbo_;
	mygl3::Texture2D shadow_texture_;

public:
	void Initialize();
	void Update();
	const mygl3::Texture2D &Get() const { return shadow_texture_; };
};

#endif //SCENETEST_SHADOWMAP_HPP
