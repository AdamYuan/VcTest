//
// Created by adamyuan on 4/24/18.
//

#include "Skybox.hpp"
#include "Resources.hpp"
#include <iostream>

void Skybox::Initialize()
{
	texture_.Initialize();
	texture_.Bind();
	for(int i = 0; i < 6; ++i)
	{
		std::string filename{"skybox/"};
		filename += std::to_string(i) + ".png";
		mygl3::ImageLoader skybox{filename.c_str()};
		const mygl3::ImageInfo &info = skybox.GetInfo();
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, info.internal_format,
					 info.width, info.height, 0,
					 info.format, info.type, info.data);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	float skybox_vertices[] =
			{
					// positions
					-1.0f,  1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,

					-1.0f, -1.0f,  1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f, -1.0f,
					-1.0f,  1.0f,  1.0f,
					-1.0f, -1.0f,  1.0f,

					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f,  1.0f,
					1.0f,  1.0f,  1.0f,
					1.0f,  1.0f,  1.0f,
					1.0f,  1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,

					-1.0f, -1.0f,  1.0f,
					-1.0f,  1.0f,  1.0f,
					1.0f,  1.0f,  1.0f,
					1.0f,  1.0f,  1.0f,
					1.0f, -1.0f,  1.0f,
					-1.0f, -1.0f,  1.0f,

					-1.0f,  1.0f, -1.0f,
					1.0f,  1.0f, -1.0f,
					1.0f,  1.0f,  1.0f,
					1.0f,  1.0f,  1.0f,
					-1.0f,  1.0f,  1.0f,
					-1.0f,  1.0f, -1.0f,

					-1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f,  1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f,  1.0f,
					1.0f, -1.0f,  1.0f
			};
	object_.Initialize();
	object_.SetData(skybox_vertices, 108, GL_STATIC_DRAW);
	object_.SetAttributes(0, 3);

	shader_.Initialize();
	shader_.LoadFromFile("shaders/skybox.vert", GL_VERTEX_SHADER);
	shader_.LoadFromFile("shaders/skybox.frag", GL_FRAGMENT_SHADER);
	unif_projection_ = shader_.GetUniform("uProjection");
	unif_view_ = shader_.GetUniform("uView");
}

void Skybox::Render()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	glCullFace(GL_BACK);

	glm::mat4 view = glm::mat4(glm::mat3(res::cam_view));
	shader_.SetMat4(unif_view_, view);
	shader_.SetMat4(unif_projection_, res::cam_projection);

	texture_.Bind(GL_TEXTURE0);

	shader_.Use();
	object_.Render(GL_TRIANGLES);

	glDepthFunc(GL_LESS); // set depth function back to default
}
