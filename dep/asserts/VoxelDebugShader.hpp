//GENERATED BY GLSL_GEN
#ifndef VOXELDEBUGSHADER_HPP
#define VOXELDEBUGSHADER_HPP
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <GL/gl3w.h>
namespace asserts {
class VoxelDebugShader {
private:
	GLuint program_{0};
public:
	void Use() const { glUseProgram(program_); }
	GLuint Get() const { return program_; }
	VoxelDebugShader() = default;
	~VoxelDebugShader(){ if(program_ != 0) glDeleteProgram(program_); }
	VoxelDebugShader(const VoxelDebugShader &) = delete;
	VoxelDebugShader& operator= (const VoxelDebugShader &) = delete;
private:
	GLint unif_uView;
	GLint unif_uProjection;
public:
	void Initialize() {
		GLuint shader;
		program_ = glCreateProgram();
		const char *GL_FRAGMENT_SHADER_src = "#version 450 core\n\nout vec4 FragColor;\n\nin vec3 vTexcoordsFront, vTexcoordsBack;\n\nlayout (binding = 0) uniform sampler3D uVoxelAlbedo;\n\nvoid main()\n{\n	FragColor = texture(uVoxelAlbedo, gl_FrontFacing ? vTexcoordsFront : vTexcoordsBack);\n	if(FragColor.a < 0.1f)\n		discard;\n	vec3 mapped = vec3(1.0f) - exp(-FragColor.rgb * 1.5f);\n	mapped = pow(mapped, vec3(1.0f / 2.2f));\n	FragColor = vec4(mapped, 1.0f);\n}\n";
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &GL_FRAGMENT_SHADER_src, nullptr);
		glCompileShader(shader);
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		const char *GL_VERTEX_SHADER_src = "#version 450 core\nlayout (location = 0) in vec3 aPosition;\nlayout (location = 1) in vec3 aTexcoordsFront;\nlayout (location = 2) in vec3 aTexcoordsBack;\n\nout vec3 vTexcoordsFront, vTexcoordsBack;\n\nuniform mat4 uProjection, uView;\n\nvoid main()\n{\n	vTexcoordsFront = aTexcoordsFront;\n	vTexcoordsBack = aTexcoordsBack;\n	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);\n}\n";
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &GL_VERTEX_SHADER_src, nullptr);
		glCompileShader(shader);
		glAttachShader(program_, shader);
		glLinkProgram(program_);
		glDeleteShader(shader);
		unif_uView = glGetUniformLocation(program_, "uView");
		unif_uProjection = glGetUniformLocation(program_, "uProjection");
	}
	void SetUView(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uView, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUViewLocation() const { return unif_uView; };
	void SetUProjection(const glm::mat4 &v) { glProgramUniformMatrix4fv(program_, unif_uProjection, 1, GL_FALSE, glm::value_ptr(v)); }
	GLint GetUProjectionLocation() const { return unif_uProjection; };
};
}
#endif
