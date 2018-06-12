//
// Created by adamyuan on 4/17/18.
//

#include <GL/gl3w.h>
#include "Application.hpp"
#include "Resources.hpp"
#include "Config.hpp"

void Application::init_window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window_ = glfwCreateWindow(kWidth, kHeight, "", nullptr, nullptr);
	glfwMakeContextCurrent(window_);
	glfwSetWindowUserPointer(window_, (void*)this);

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetKeyCallback(window_, key_callback);
	glfwSetWindowFocusCallback(window_, focus_callback);

	gl3wInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Application::Application()
{
	init_window();
	debug_voxel_ = show_edge_ = false;
	control_ = indirect_trace_ = show_albedo_ = true;
	camera_.Position() = kInitialPos;
	res::Initialize();
	renderer_.Initialize();
}

void Application::Run()
{
	char title[512];
	while(!glfwWindowShouldClose(window_))
	{
		framerate_.Update();

		if(control_) cam_control();

		renderer_.Render(debug_voxel_, indirect_trace_, show_albedo_, show_edge_);

		glfwSwapBuffers(window_);
		glfwPollEvents();

		sprintf(title, "%f\n", framerate_.GetFps());
		glfwSetWindowTitle(window_, title);
	}
}

void Application::cam_control()
{
	if(glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
		camera_.MoveForward(framerate_.GetDelta() * kSpeed, 0.0f);
	if(glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
		camera_.MoveForward(framerate_.GetDelta() * kSpeed, 90.0f);
	if(glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
		camera_.MoveForward(framerate_.GetDelta() * kSpeed, -90.0f);
	if(glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
		camera_.MoveForward(framerate_.GetDelta() * kSpeed, 180.0f);
	if(glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera_.MoveUp(framerate_.GetDelta() * kSpeed);
	if(glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera_.MoveUp(-framerate_.GetDelta() * kSpeed);

	double mouse_x, mouse_y;
	glfwGetCursorPos(window_, &mouse_x, &mouse_y);
	camera_.MouseControl((float)(mouse_x - kMouseX), (float)(mouse_y - kMouseY), .3f);
	glfwSetCursorPos(window_, kMouseX, kMouseY);

	res::cam_view = camera_.GetMatrix();
	res::cam_pos = camera_.GetPosition();
}

void Application::key_callback(GLFWwindow *window, int key, int, int action, int)
{
	auto *app = (Application *)glfwGetWindowUserPointer(window);
	if(action == GLFW_PRESS)
	{
		if(key == GLFW_KEY_X)
			app->debug_voxel_ = !app->debug_voxel_;
		else if(key == GLFW_KEY_V)
			app->indirect_trace_ = !app->indirect_trace_;
		else if(key == GLFW_KEY_C)
			app->show_albedo_ = !app->show_albedo_;
		else if(key == GLFW_KEY_E)
			app->show_edge_ = !app->show_edge_;
		else if(key == GLFW_KEY_ESCAPE)
		{
			app->control_ = !app->control_;
			glfwSetCursorPos(window, kMouseX, kMouseY);
			glfwSetInputMode(window, GLFW_CURSOR, app->control_ ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
		}
	}
}

void Application::focus_callback(GLFWwindow *window, int focused)
{
	auto *app = (Application *)glfwGetWindowUserPointer(window);
	if(focused == GLFW_FALSE)
	{
		app->control_ = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
