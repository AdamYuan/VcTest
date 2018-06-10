//
// Created by adamyuan on 4/17/18.
//

#ifndef SCENETEST_APPLICATION_HPP
#define SCENETEST_APPLICATION_HPP

#include <mygl3/utils/camera.hpp>
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <mygl3/utils/framerate.hpp>

class Application
{
private:
	GLFWwindow *window_;
	mygl3::Camera camera_;
	mygl3::Framerate framerate_;
	Renderer renderer_;
	bool indirect_trace_, show_albedo_, show_edge_, control_;
	void init_window();
	void cam_control();
	static void key_callback(GLFWwindow *window, int key, int, int action, int);
	static void focus_callback(GLFWwindow *window, int focused);
public:
	Application();
	void Run();
};


#endif //SCENETEST_APPLICATION_HPP
