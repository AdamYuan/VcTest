//
// Created by adamyuan on 4/18/18.
//

#include "Model.hpp"
#include "Config.hpp"
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void Mesh::Load(const std::vector<Vertex> &vertices)
{
	object.Initialize();
	object.SetData(vertices, GL_STATIC_DRAW);
	object.SetAttributes(0, 3, 1, 3, 2, 2);
}

void Model::Load(const char *filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename))
		return;

	if (!err.empty())
	{
		printf("Error when loading model: %s\n", err.c_str());
		return;
	}

	for(const auto &shape : shapes)
	{
		std::vector<Vertex> vertices;
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex = {};

			vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
			};
			vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
			};
			vertex.texcoords = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};
			vertex.position /= kScale;

			vertices.push_back(vertex);
		}

		int material_id = shape.mesh.material_ids.front();

		std::string str{materials[material_id].diffuse_texname};
		auto iter = textures_.find(str);
		if(iter == textures_.end())
		{
			textures_.insert(std::make_pair(str, mygl3::Texture2D{}));
			iter = textures_.find(str);
			iter->second.Initialize();
			iter->second.Load(mygl3::ImageLoader(str.c_str()).GetInfo(), true);
			iter->second.GenerateMipmap();
			iter->second.SetWrapFilter(GL_REPEAT);
			iter->second.SetSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		}

		meshes_.emplace_back();
		meshes_.back().Load(vertices);
		meshes_.back().diffuse_texture = iter->second.Get();
	}
}
/*bool Model::process_texture(const aiMaterial *material, aiTextureType type, GLuint *texture)
{
	if(material->GetTextureCount(type))
	{
		aiString ai_str;
		material->GetTexture(type, 0, &ai_str); //only load one texture
		std::string str{ai_str.C_Str()};
		auto iter = textures_.find(str);
		if(iter == textures_.end())
		{
			textures_.insert(std::make_pair(str, mygl3::Texture2D{}));
			iter = textures_.find(str);
			iter->second.Initialize();
			iter->second.Load(mygl3::ImageLoader(str.c_str()).GetInfo(), true);
			iter->second.GenerateMipmap();
			iter->second.SetWrapFilter(GL_REPEAT);
			iter->second.SetSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
			glTextureParameterf(iter->second.Get(), GL_TEXTURE_MAX_ANISOTROPY, 8.0f);
		}
		*texture = iter->second.Get();
		return true;
	}
	return false;
}
*/
void Model::Render() const
{
	for(const Mesh &m : meshes_)
	{
		glBindTextureUnit(0, m.diffuse_texture);
		m.object.Render(GL_TRIANGLES);
	}
}

