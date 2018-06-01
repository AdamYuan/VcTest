//
// Created by adamyuan on 4/18/18.
//

#include "Model.hpp"
#include "Config.hpp"
#include <GLFW/glfw3.h>

void Mesh::Load(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices)
{
	object.Initialize();
	object.SetData(vertices, GL_STATIC_DRAW);
	object.SetIndices(indices, GL_STATIC_DRAW);
	object.SetAttributes(0, 3, 1, 3, 2, 3, 3, 3, 4, 2);
}

void Model::Load(const char *filename)
{
	min_pos_ = glm::vec3(FLT_MAX);
	max_pos_ = glm::vec3(FLT_MIN);
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace |
													   aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
	if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
	{
		printf("Error when loading model: %s\n", importer.GetErrorString());
		return;
	}
	process_node(scene->mRootNode, scene);
	//printf("%f %f %f\n%f %f %f\n", min_pos_.x, min_pos_.y, min_pos_.z, max_pos_.x, max_pos_.y, max_pos_.z);
}

void Model::process_node(const aiNode *node, const aiScene *scene)
{
	for(size_t i = 0; i < node->mNumMeshes; ++i)
	{
		const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		process_mesh(mesh, scene);
	}
	for(size_t i = 0; i < node->mNumChildren; ++i)
		process_node(node->mChildren[i], scene);
}

void Model::process_mesh(const aiMesh *mesh, const aiScene *scene)
{
#define TO_GLM_VEC3(a) (glm::vec3((a).x, (a).y, (a).z))
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);

	for(size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex v;
		v.position = TO_GLM_VEC3(mesh->mVertices[i]);
		v.position /= kScale; //scale

		max_pos_ = glm::max(max_pos_, v.position);
		min_pos_ = glm::min(min_pos_, v.position);

		v.normal = TO_GLM_VEC3(mesh->mNormals[i]);
		v.tangent = TO_GLM_VEC3(mesh->mTangents[i]);
		v.bitangent = TO_GLM_VEC3(mesh->mBitangents[i]);

		if(mesh->mTextureCoords[0])
		{
			v.texcoords.x = mesh->mTextureCoords[0][i].x;
			v.texcoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			v.texcoords.x = v.texcoords.y = 0;
		vertices.push_back(v);
	}
#undef TO_GLM_VEC3

	for(size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace &face = mesh->mFaces[i];
		for(size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	meshes_.emplace_back();
	meshes_.back().Load(vertices, indices);

	if(mesh->mMaterialIndex >= 0)
	{
		const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		process_texture(material, aiTextureType_DIFFUSE, &meshes_.back().diffuse_texture);
		meshes_.back().have_normal_texture =
				process_texture(material, aiTextureType_NORMALS, &meshes_.back().normal_texture);
	}
}

bool Model::process_texture(const aiMaterial *material, aiTextureType type, GLuint *texture)
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

void Model::Render(GLint unif_have_normal_texture) const
{
	for(const Mesh &m : meshes_)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m.diffuse_texture);
		if(unif_have_normal_texture != -1)
		{
			glUniform1i(unif_have_normal_texture, m.have_normal_texture);

			if(m.have_normal_texture)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m.normal_texture);
			}
		}

		m.object.Render(GL_TRIANGLES);
	}
}

