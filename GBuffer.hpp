//
// Created by adamyuan on 5/21/18.
//

#ifndef VXGITEST_GBUFFER_HPP
#define VXGITEST_GBUFFER_HPP

#include <mygl3/texture.hpp>
#include <mygl3/shader.hpp>
#include <mygl3/framebuffer.hpp>

class GBuffer
{
private:
	mygl3::Texture2D albedo_, normal_, position_;
	mygl3::FrameBuffer fbo_;
	mygl3::RenderBuffer rbo_;
	mygl3::Shader shader_; GLint unif_projection_, unif_view_, unif_have_normal_texture_;
public:
	void Initialize();
	void Update();
	const mygl3::Texture2D &GetPositionTexture() const { return position_; }
	const mygl3::Texture2D &GetNormalTexture() const { return normal_; }
	const mygl3::Texture2D &GetAlbedoTexture() const { return albedo_; }
};


#endif //VXGITEST_GBUFFER_HPP
