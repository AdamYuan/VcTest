//GENERATED BY GLSL_GEN
#ifndef VOXELDIRECTLIGHTSHADER_HPP
#define VOXELDIRECTLIGHTSHADER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <GL/gl3w.h>
namespace asserts {
class VoxelDirectLightShader {
private:
	GLuint program_{0};
public:
	void Use() const { glUseProgram(program_); }
	GLuint Get() const { return program_; }
	VoxelDirectLightShader() = default;
	~VoxelDirectLightShader(){ if(program_ != 0) glDeleteProgram(program_); }
	VoxelDirectLightShader(const VoxelDirectLightShader &) = delete;
	VoxelDirectLightShader& operator= (const VoxelDirectLightShader &) = delete;
private:
	GLint unif_uVoxelDimension;
	GLint unif_uLightMatrix;
	GLint unif_uLightDir;
	GLint unif_uVoxelGridRangeMin;
	GLint unif_uVoxelWorldSize;
public:
	void Initialize() {
		GLuint shader;
		program_ = glCreateProgram();
		std::ifstream in; std::string str;
		char log[100000]; int success;
		in.open("shaders/voxel_direct_light.comp");
		if(in.is_open()) {
			std::getline(in, str, '\0');
			in.close();
		} else {
			str.clear();
			printf("[GLSLGEN ERROR] failed to load shaders/voxel_direct_light.comp\n");
		}
		const char *GL_COMPUTE_SHADER_src = str.c_str();
		shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(shader, 1, &GL_COMPUTE_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("[GLSLGEN ERROR] compile error in shaders/voxel_direct_light.comp:\n%s\n", log);
		}
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		unif_uVoxelDimension = glGetUniformLocation(program_, "uVoxelDimension");
		unif_uLightMatrix = glGetUniformLocation(program_, "uLightMatrix");
		unif_uLightDir = glGetUniformLocation(program_, "uLightDir");
		unif_uVoxelGridRangeMin = glGetUniformLocation(program_, "uVoxelGridRangeMin");
		unif_uVoxelWorldSize = glGetUniformLocation(program_, "uVoxelWorldSize");
	}
	void SetUVoxelDimension(const glm::ivec3 &v) { glProgramUniform3iv(program_, unif_uVoxelDimension, 1, glm::value_ptr(v)); }
	GLint GetUVoxelDimensionLocation() const { return unif_uVoxelDimension; };
	void SetULightMatrix(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uLightMatrix, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetULightMatrixLocation() const { return unif_uLightMatrix; };
	void SetULightDir(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uLightDir, 1, glm::value_ptr(v)); }
	GLint GetULightDirLocation() const { return unif_uLightDir; };
	void SetUVoxelGridRangeMin(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uVoxelGridRangeMin, 1, glm::value_ptr(v)); }
	GLint GetUVoxelGridRangeMinLocation() const { return unif_uVoxelGridRangeMin; };
	void SetUVoxelWorldSize(GLfloat v) { glProgramUniform1f(program_, unif_uVoxelWorldSize, v); }
	GLint GetUVoxelWorldSizeLocation() const { return unif_uVoxelWorldSize; };
};
}
#endif
