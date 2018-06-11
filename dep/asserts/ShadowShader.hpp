//GENERATED BY GLSL_GEN
#ifndef SHADOWSHADER_HPP
#define SHADOWSHADER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <GL/gl3w.h>
namespace asserts {
class ShadowShader {
private:
	GLuint program_{0};
public:
	void Use() const { glUseProgram(program_); }
	GLuint Get() const { return program_; }
	ShadowShader() = default;
	~ShadowShader(){ if(program_ != 0) glDeleteProgram(program_); }
	ShadowShader(const ShadowShader &) = delete;
	ShadowShader& operator= (const ShadowShader &) = delete;
private:
	GLint unif_uLightMatrix;
public:
	void Initialize() {
		GLuint shader;
		program_ = glCreateProgram();
		std::ifstream in; std::string str;
		char log[100000]; int success;
		in.open("shaders/shadow.frag");
		if(in.is_open()) {
			std::getline(in, str, '\0');
			in.close();
		} else {
			str.clear();
			printf("[GLSLGEN ERROR] failed to load shaders/shadow.frag\n");
		}
		const char *GL_FRAGMENT_SHADER_src = str.c_str();
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &GL_FRAGMENT_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("[GLSLGEN ERROR] compile error in shaders/shadow.frag:\n%s\n", log);
		}
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		in.open("shaders/shadow.vert");
		if(in.is_open()) {
			std::getline(in, str, '\0');
			in.close();
		} else {
			str.clear();
			printf("[GLSLGEN ERROR] failed to load shaders/shadow.vert\n");
		}
		const char *GL_VERTEX_SHADER_src = str.c_str();
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &GL_VERTEX_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("[GLSLGEN ERROR] compile error in shaders/shadow.vert:\n%s\n", log);
		}
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		unif_uLightMatrix = glGetUniformLocation(program_, "uLightMatrix");
	}
	void SetULightMatrix(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uLightMatrix, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetULightMatrixLocation() const { return unif_uLightMatrix; };
};
}
#endif
