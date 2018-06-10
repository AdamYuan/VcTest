//GENERATED BY GLSL_GEN
#ifndef FINALSHADER_HPP
#define FINALSHADER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <GL/gl3w.h>
namespace asserts {
class FinalShader {
private:
	GLuint program_{0};
public:
	void Use() const { glUseProgram(program_); }
	GLuint Get() const { return program_; }
	FinalShader() = default;
	~FinalShader(){ if(program_ != 0) glDeleteProgram(program_); }
	FinalShader(const FinalShader &) = delete;
	FinalShader& operator= (const FinalShader &) = delete;
private:
	GLint unif_uView;
	GLint unif_uProjection;
	GLint unif_uLightMatrix;
	GLint unif_uLightDir;
	GLint unif_uVoxelDimension;
	GLint unif_uVoxelGridRangeMin;
	GLint unif_uVoxelGridRangeMax;
	GLint unif_uVoxelWorldSize;
	GLint unif_uEnableIndirectTrace;
	GLint unif_uShowAlbedo;
	GLint unif_uDebugVoxel;
	GLint unif_uShowEdge;
	GLint unif_uCamPosition;
	GLint unif_uResolution;
public:
	void Initialize() {
		GLuint shader;
		program_ = glCreateProgram();
		std::ifstream in; std::string str;
		char log[100000]; int success;
		in.open("shaders/final_pass.frag");
		std::getline(in, str, '\0');
		in.close();
		const char *GL_FRAGMENT_SHADER_src = str.c_str();
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &GL_FRAGMENT_SHADER_src, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 100000, nullptr, log);
			printf("compile error in shaders/final_pass.frag:\n%s\n", log);
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
		unif_uLightMatrix = glGetUniformLocation(program_, "uLightMatrix");
		unif_uLightDir = glGetUniformLocation(program_, "uLightDir");
		unif_uVoxelDimension = glGetUniformLocation(program_, "uVoxelDimension");
		unif_uVoxelGridRangeMin = glGetUniformLocation(program_, "uVoxelGridRangeMin");
		unif_uVoxelGridRangeMax = glGetUniformLocation(program_, "uVoxelGridRangeMax");
		unif_uVoxelWorldSize = glGetUniformLocation(program_, "uVoxelWorldSize");
		unif_uEnableIndirectTrace = glGetUniformLocation(program_, "uEnableIndirectTrace");
		unif_uShowAlbedo = glGetUniformLocation(program_, "uShowAlbedo");
		unif_uDebugVoxel = glGetUniformLocation(program_, "uDebugVoxel");
		unif_uShowEdge = glGetUniformLocation(program_, "uShowEdge");
		unif_uCamPosition = glGetUniformLocation(program_, "uCamPosition");
		unif_uResolution = glGetUniformLocation(program_, "uResolution");
	}
	void SetUView(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uView, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUViewLocation() const { return unif_uView; };
	void SetUProjection(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uProjection, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUProjectionLocation() const { return unif_uProjection; };
	void SetULightMatrix(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uLightMatrix, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetULightMatrixLocation() const { return unif_uLightMatrix; };
	void SetULightDir(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uLightDir, 1, glm::value_ptr(v)); }
	GLint GetULightDirLocation() const { return unif_uLightDir; };
	void SetUVoxelDimension(const glm::ivec3 &v) { glProgramUniform3iv(program_, unif_uVoxelDimension, 1, glm::value_ptr(v)); }
	GLint GetUVoxelDimensionLocation() const { return unif_uVoxelDimension; };
	void SetUVoxelGridRangeMin(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uVoxelGridRangeMin, 1, glm::value_ptr(v)); }
	GLint GetUVoxelGridRangeMinLocation() const { return unif_uVoxelGridRangeMin; };
	void SetUVoxelGridRangeMax(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uVoxelGridRangeMax, 1, glm::value_ptr(v)); }
	GLint GetUVoxelGridRangeMaxLocation() const { return unif_uVoxelGridRangeMax; };
	void SetUVoxelWorldSize(GLfloat v) { glProgramUniform1f(program_, unif_uVoxelWorldSize, v); }
	GLint GetUVoxelWorldSizeLocation() const { return unif_uVoxelWorldSize; };
	void SetUEnableIndirectTrace(bool v) { glProgramUniform1i(program_, unif_uEnableIndirectTrace, v); }
	GLint GetUEnableIndirectTraceLocation() const { return unif_uEnableIndirectTrace; };
	void SetUShowAlbedo(bool v) { glProgramUniform1i(program_, unif_uShowAlbedo, v); }
	GLint GetUShowAlbedoLocation() const { return unif_uShowAlbedo; };
	void SetUDebugVoxel(bool v) { glProgramUniform1i(program_, unif_uDebugVoxel, v); }
	GLint GetUDebugVoxelLocation() const { return unif_uDebugVoxel; };
	void SetUShowEdge(bool v) { glProgramUniform1i(program_, unif_uShowEdge, v); }
	GLint GetUShowEdgeLocation() const { return unif_uShowEdge; };
	void SetUCamPosition(const glm::vec3 &v) { glProgramUniform3fv(program_, unif_uCamPosition, 1, glm::value_ptr(v)); }
	GLint GetUCamPositionLocation() const { return unif_uCamPosition; };
	void SetUResolution(const glm::ivec2 &v) { glProgramUniform2iv(program_, unif_uResolution, 1, glm::value_ptr(v)); }
	GLint GetUResolutionLocation() const { return unif_uResolution; };
};
}
#endif
