//
// Created by adamyuan on 5/21/18.
//

#include "GBuffer.hpp"
#include "Config.hpp"
#include "Resources.hpp"

void GBuffer::Initialize()
{
	position_.Initialize();
	position_.Load(mygl3::ImageInfo(kWidth, kHeight, 0, GL_RGBA32F, 0, 0, nullptr), false);
	position_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	position_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	normal_.Initialize();
	normal_.Load(mygl3::ImageInfo(kWidth, kHeight, 0, GL_RGB8, 0, 0, nullptr), false);
	normal_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	normal_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	albedo_.Initialize();
	albedo_.Load(mygl3::ImageInfo(kWidth, kHeight, 0, GL_RGBA8, 0, 0, nullptr), false);
	albedo_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	albedo_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	half_position_.Initialize();
	half_position_.Load(mygl3::ImageInfo(kHalfWidth, kHalfHeight, 0, GL_RGBA32F, 0, 0, nullptr), false);
	half_position_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	half_position_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	half_normal_.Initialize();
	half_normal_.Load(mygl3::ImageInfo(kHalfWidth, kHalfHeight, 0, GL_RGB8, 0, 0, nullptr), false);
	half_normal_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	half_normal_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	gbuffer_rbo_.Initialize();
	gbuffer_rbo_.Load(GL_DEPTH_COMPONENT, kWidth, kHeight);

	gbuffer_fbo_.Initialize();
	gbuffer_fbo_.AttachTexture(position_, GL_COLOR_ATTACHMENT0);
	gbuffer_fbo_.AttachTexture(normal_, GL_COLOR_ATTACHMENT1);
	gbuffer_fbo_.AttachTexture(albedo_, GL_COLOR_ATTACHMENT2);
	gbuffer_fbo_.AttachRenderbuffer(gbuffer_rbo_, GL_DEPTH_ATTACHMENT);
	GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glNamedFramebufferDrawBuffers(gbuffer_fbo_.Get(), 3, attachments);

	half_fbo_.Initialize();
	half_fbo_.AttachTexture(half_position_, GL_COLOR_ATTACHMENT0);
	half_fbo_.AttachTexture(half_normal_, GL_COLOR_ATTACHMENT1);
	glNamedFramebufferDrawBuffers(half_fbo_.Get(), 2, attachments);

	gbuffer_shader_.Initialize();
	gbuffer_shader_.SetUProjection(res::cam_projection);

	half_shader_.Initialize();
}

void GBuffer::Update()
{
	{
		mygl3::FrameBufferBinder binder(gbuffer_fbo_);

		glViewport(0, 0, kWidth, kHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);

		gbuffer_shader_.Use();
		gbuffer_shader_.SetUView(res::cam_view);

		res::sponza_model.Render();
	}

	{
		mygl3::FrameBufferBinder binder(half_fbo_);

		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, kHalfWidth, kHalfHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		position_.Bind(0);
		normal_.Bind(1);
		half_shader_.Use();
		res::quad_object.Render(GL_TRIANGLES);

		glEnable(GL_DEPTH_TEST);
	}
}
