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
	normal_.Load(mygl3::ImageInfo(kWidth, kHeight, 0, GL_RGBA8, 0, 0, nullptr), false);
	normal_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	normal_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	albedo_.Initialize();
	albedo_.Load(mygl3::ImageInfo(kWidth, kHeight, 0, GL_RGBA8, 0, 0, nullptr), false);
	albedo_.SetWrapFilter(GL_CLAMP_TO_EDGE);
	albedo_.SetSizeFilter(GL_NEAREST, GL_NEAREST);

	rbo_.Initialize();
	rbo_.Load(GL_DEPTH_COMPONENT, kWidth, kHeight);

	fbo_.Initialize();
	fbo_.AttachTexture(position_, GL_COLOR_ATTACHMENT0);
	fbo_.AttachTexture(normal_, GL_COLOR_ATTACHMENT1);
	fbo_.AttachTexture(albedo_, GL_COLOR_ATTACHMENT2);
	fbo_.AttachRenderbuffer(rbo_, GL_DEPTH_ATTACHMENT);

	GLenum attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glNamedFramebufferDrawBuffers(fbo_.Get(), 3, attachments);

	shader_.Initialize();
	shader_.SetUProjection(res::cam_projection);
}

void GBuffer::Update()
{
	mygl3::FrameBufferBinder binder(fbo_);

	glViewport(0, 0, kWidth, kHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	shader_.Use();
	shader_.SetUView(res::cam_view);

	res::sponza_model.Render(shader_.GetUHaveNormalTextureLocation());
}
