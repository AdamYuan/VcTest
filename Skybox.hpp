//
// Created by adamyuan on 4/24/18.
//

#ifndef SCENETEST_SKYBOX_HPP
#define SCENETEST_SKYBOX_HPP

#include <mygl3/texture.hpp>
#include <mygl3/vertexobject.hpp>
#include <asserts/SkyboxShader.hpp>

class Skybox
{
private:
	mygl3::Texture<GL_TEXTURE_CUBE_MAP> texture_;
	mygl3::VertexObject<false> object_;
	asserts::SkyboxShader shader_;
public:
	void Initialize();
	const mygl3::Texture<GL_TEXTURE_CUBE_MAP> &Get() const { return texture_; }
	void Render();
};

#endif //SCENETEST_SKYBOX_HPP
