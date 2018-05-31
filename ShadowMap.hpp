//
// Created by adamyuan on 4/23/18.
//

#ifndef SCENETEST_SHADOWMAP_HPP
#define SCENETEST_SHADOWMAP_HPP

#include <mygl3/framebuffer.hpp>
#include "Model.hpp"

class ShadowMap
{
private:
	//shadow map
	mygl3::Shader shadow_shader_; GLint shadow_unif_light_matrix_;
	mygl3::FrameBuffer shadow_fbo_;
	mygl3::Texture2D shadow_texture_;

public:
	void Initialize();
	void Update();
	const mygl3::Texture2D &Get() const { return shadow_texture_; };
};

#endif //SCENETEST_SHADOWMAP_HPP
