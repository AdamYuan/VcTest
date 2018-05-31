//
// Created by adamyuan on 4/23/18.
//

#ifndef SCENETEST_RESOURCES_HPP
#define SCENETEST_RESOURCES_HPP

#include <mygl3/framebuffer.hpp>
#include "Model.hpp"

namespace res
{
	//matrices
	extern glm::mat4 cam_projection, cam_view, light_matrix;

	extern glm::vec3 cam_pos;

	extern Model sponza_model;
	//just a quad
	extern mygl3::VertexObject<false> quad_object;

	void Initialize();
};

#endif //SCENETEST_RESOURCES_HPP
