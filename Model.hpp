//
// Created by adamyuan on 4/18/18.
//

#ifndef SCENETEST_SCENE_HPP
#define SCENETEST_SCENE_HPP

#include <mygl3/vertexobject.hpp>
#include <mygl3/texture.hpp>
#include <mygl3/shader.hpp>
#include <mygl3/utils/camera.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	mygl3::VertexObject<true> object;
	void Load(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices);
};

class Model
{
private:
	std::vector<Mesh> meshes_;
	std::map<std::string, mygl3::Texture2D> textures_;
	void process_node(const aiNode *node, const aiScene *scene);
	void process_mesh(const aiMesh *mesh, const aiScene *scene);
	bool process_texture(const aiMaterial *material, aiTextureType type, GLuint *texture);

	glm::vec3 min_pos_, max_pos_;

public:
	void Load(const char *filename);
	void Render() const;
};


#endif //SCENETEST_SCENE_HPP
