//
// Created by adamyuan on 4/23/18.
//

#include "Resources.hpp"
#include "Config.hpp"

namespace res
{
	//matrices
	glm::mat4 cam_projection, cam_view, light_matrix;

	glm::vec3 cam_pos, light_dir;
	Model sponza_model;
	//just a quad
	mygl3::VertexObject<false> quad_object;

	void Initialize()
	{
		sponza_model.Load("models/sponza.obj");
		//matrices
		cam_projection = glm::perspective(glm::radians(45.0f), (float)kWidth / (float)kHeight, 0.1f, 100.0f);

		constexpr float quad_vertices[] { -1.0f, -1.0f, 0.0f, 0.0f,
										  1.0f, -1.0f, 1.0f, 0.0f,
										  1.0f, 1.0f, 1.0f, 1.0f,
										  1.0f, 1.0f, 1.0f, 1.0f,
										  -1.0f, 1.0f, 0.0f, 1.0f,
										  -1.0f, -1.0f, 0.0f, 0.0f };
		quad_object.Initialize();
		quad_object.SetData(quad_vertices, 24, GL_STATIC_DRAW);
		quad_object.SetAttributes(0, 2, 1, 2);
	}

	void UpdateCam(const mygl3::Camera &cam)
	{
		cam_pos = cam.GetPosition();
		cam_view = cam.GetMatrix();
	}

	void UpdateLight(const glm::vec3 &sun_pos)
	{
		light_dir = glm::normalize(-sun_pos);
		glm::vec3 up{0.0f, 1.0f, 0.0f};
		if(light_dir.x == 0.0f && light_dir.z == 0.0f)
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::mat4 light_view = glm::lookAt(glm::vec3(0.0f), light_dir, up);
		glm::mat4 light_projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
		light_matrix = light_projection * light_view;
	}
}
