//
// Created by adamyuan on 5/21/18.
//

#ifndef VXGITEST_GBUFFER_HPP
#define VXGITEST_GBUFFER_HPP

#include <asserts/GBufferShader.hpp>
#include <asserts/HalfShader.hpp>
#include <mygl3/texture.hpp>
#include <mygl3/framebuffer.hpp>

class GBuffer
{
private:
	mygl3::Texture2D albedo_, normal_, position_, half_position_, half_normal_;
	mygl3::FrameBuffer gbuffer_fbo_, half_fbo_;
	mygl3::RenderBuffer gbuffer_rbo_;
	asserts::GBufferShader gbuffer_shader_;
	asserts::HalfShader half_shader_;
public:
	void Initialize();
	void Update();
	const mygl3::Texture2D &GetPositionTexture() const { return position_; }
	const mygl3::Texture2D &GetNormalTexture() const { return normal_; }
	const mygl3::Texture2D &GetAlbedoTexture() const { return albedo_; }

	const mygl3::Texture2D &GetHalfPositionTexture() const { return half_position_; }
	const mygl3::Texture2D &GetHalfNormalTexture() const { return half_normal_; }
};


#endif //VXGITEST_GBUFFER_HPP
