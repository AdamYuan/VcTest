//
// Created by adamyuan on 4/18/18.
//

#include "Model.hpp"
#include "Config.hpp"
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void Mesh::Load()
{
	object.Initialize();
	object.SetData(vertices, GL_STATIC_DRAW);
	object.SetAttributes(0, 3, 1, 3, 2, 2);

	vertices.clear();
	vertices.shrink_to_fit();
}

void Model::Load(const char *filename)
{
	double start = glfwGetTime();

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

	meshes_.resize(materials.size());

	// Loop over shapes
	for(const auto &shape : shapes)
	{
		size_t index_offset = 0, face = 0;

		// Loop over faces(polygon)
		for(const auto &fv : shape.mesh.num_face_vertices)
		{
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t index = shape.mesh.indices[index_offset + v];

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

				meshes_[shape.mesh.material_ids[face]].vertices.push_back(vertex);
			}
			index_offset += fv;
			face ++;
		}
	}

	for(size_t i = 0; i < materials.size(); ++i)
	{
		meshes_[i].diffuse_texture = load_texture(materials[i].diffuse_texname);
		meshes_[i].Load();
	}

	printf("Loading model lasted: %lf sec\n", glfwGetTime() - start);
}
void Model::Render() const
{
	for(const Mesh &m : meshes_)
	{
		glBindTextureUnit(0, m.diffuse_texture);
		m.object.Render(GL_TRIANGLES);
	}
}

GLuint Model::load_texture(const std::string &filename)
{
	auto iter = textures_.find(filename);
	if(iter == textures_.end())
	{
		textures_.insert(std::make_pair(filename, mygl3::Texture2D{}));
		iter = textures_.find(filename);
		iter->second.Initialize();
		iter->second.Load(mygl3::ImageLoader(filename.c_str()).GetInfo(), true);
		iter->second.GenerateMipmap();
		iter->second.SetWrapFilter(GL_REPEAT);
		iter->second.SetSizeFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	}
	return iter->second.Get();
}

