//
// Created by adamyuan on 4/23/18.
//

#include "ShadowMap.hpp"
#include "Config.hpp"
#include "Resources.hpp"

void ShadowMap::Initialize()
{
	//depth shader
	shadow_shader_.Initialize();
	shadow_shader_.LoadFromFile("shaders/shadow.frag", GL_FRAGMENT_SHADER);
	shadow_shader_.LoadFromFile("shaders/shadow.vert", GL_VERTEX_SHADER);
	shadow_unif_light_matrix_ = shadow_shader_.GetUniform("uLightMatrix");

	shadow_texture_.Initialize();
	shadow_texture_.Load(mygl3::ImageInfo(kShadowWidth, kShadowHeight, 0, GL_DEPTH_COMPONENT24, 0, 0, nullptr), false);
	shadow_texture_.SetSizeFilter(GL_LINEAR, GL_LINEAR);
	shadow_texture_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	glTextureParameteri(shadow_texture_.Get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTextureParameteri(shadow_texture_.Get(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	shadow_fbo_.Initialize();
	shadow_fbo_.AttachTexture(shadow_texture_, GL_DEPTH_ATTACHMENT);
	glNamedFramebufferDrawBuffer(shadow_fbo_.Get(), GL_NONE);
	glNamedFramebufferReadBuffer(shadow_fbo_.Get(), GL_NONE);
}

void ShadowMap::Update()
{
	mygl3::FrameBufferBinder binder(shadow_fbo_);
	glCullFace(GL_BACK);

	glViewport(0, 0, kShadowWidth, kShadowHeight);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadow_shader_.SetMat4(shadow_unif_light_matrix_, res::light_matrix);
	shadow_shader_.Use();
	res::sponza_model.Render();
}
