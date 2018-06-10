//
// Created by adamyuan on 4/18/18.
//

#ifndef SCENETEST_SCENE_HPP
#define SCENETEST_SCENE_HPP

#include <mygl3/vertexobject.hpp>
#include <mygl3/texture.hpp>
#include <mygl3/shader.hpp>
#include <mygl3/utils/camera.hpp>

#include <vector>
#include <map>

struct Vertex
{
	glm::vec3 position, normal;
	glm::vec2 texcoords;
};

struct Mesh
{
	GLuint diffuse_texture;
	mygl3::VertexObject<false> object;
	void Load(const std::vector<Vertex> &vertices);
};

class Model
{
private:
	std::vector<Mesh> meshes_;
	std::map<std::string, mygl3::Texture2D> textures_;

public:
	void Load(const char *filename);
	void Render() const;
};


#endif //SCENETEST_SCENE_HPP
