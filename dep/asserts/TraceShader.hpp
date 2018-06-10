//GENERATED BY GLSL_GEN
#ifndef TRACESHADER_HPP
#define TRACESHADER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <GL/gl3w.h>
namespace asserts {
class TraceShader {
private:
	GLuint program_{0};
public:
	void Use() const { glUseProgram(program_); }
	GLuint Get() const { return program_; }
	TraceShader() = default;
	~TraceShader(){ if(program_ != 0) glDeleteProgram(program_); }
	TraceShader(const TraceShader &) = delete;
	TraceShader& operator= (const TraceShader &) = delete;
private:
	GLint unif_uView;
	GLint unif_uProjection;
	GLint unif_uVoxelDimension;
	GLint unif_uVoxelGridRangeMin;
	GLint unif_uVoxelGridRangeMax;
	GLint unif_uVoxelWorldSize;
	GLint unif_uCamPosition;
public:
	void Initialize() {
		GLuint shader;
		program_ = glCreateProgram();
		std::ifstream in; std::string str;
		char log[100000]; int success;
		in.open("shaders/trace.frag");
		std::getline(in, str, '\0');
		in.close();
		const char *GL_FRAGMENT_SHADER_src = str.c_str();
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &GL_FRAGMENT_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("compile error in shaders/trace.frag:\n%s\n", log);
		}
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		in.open("shaders/trace.vert");
		std::getline(in, str, '\0');
		in.close();
		const char *GL_VERTEX_SHADER_src = str.c_str();
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &GL_VERTEX_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("compile error in shaders/trace.vert:\n%s\n", log);
		}
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		unif_uView = glGetUniformLocation(program_, "uView");
		unif_uProjection = glGetUniformLocation(program_, "uProjection");
		unif_uVoxelDimension = glGetUniformLocation(program_, "uVoxelDimension");
		unif_uVoxelGridRangeMin = glGetUniformLocation(program_, "uVoxelGridRangeMin");
		unif_uVoxelGridRangeMax = glGetUniformLocation(program_, "uVoxelGridRangeMax");
		unif_uVoxelWorldSize = glGetUniformLocation(program_, "uVoxelWorldSize");
		unif_uCamPosition = glGetUniformLocation(program_, "uCamPosition");
	}
	void SetUView(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uView, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUViewLocation() const { return unif_uView; };
	void SetUProjection(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uProjection, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUProjectionLocation() const { return unif_uProjection; };
	void SetUVoxelDimension(const glm::ivec3 &v) { glProgramUniform3iv(program_, unif_uVoxelDimension, 1, glm::value_ptr(v)); }
	GLint GetUVoxelDimensionLocation() const { return unif_uVoxelDimension; };
	void SetUVoxelGridRangeMin(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uVoxelGridRangeMin, 1, glm::value_ptr(v)); }
	GLint GetUVoxelGridRangeMinLocation() const { return unif_uVoxelGridRangeMin; };
	void SetUVoxelGridRangeMax(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uVoxelGridRangeMax, 1, glm::value_ptr(v)); }
	GLint GetUVoxelGridRangeMaxLocation() const { return unif_uVoxelGridRangeMax; };
	void SetUVoxelWorldSize(GLfloat v) { glProgramUniform1f(program_, unif_uVoxelWorldSize, v); }
	GLint GetUVoxelWorldSizeLocation() const { return unif_uVoxelWorldSize; };
	void SetUCamPosition(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uCamPosition, 1, glm::value_ptr(v)); }
	GLint GetUCamPositionLocation() const { return unif_uCamPosition; };
};
}
#endif
