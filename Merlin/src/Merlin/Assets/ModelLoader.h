#pragma once

#include "AssetManager.h"
#include "MeshAsset.h"
#include "TextureAsset.h"
#include "Material.h"
#include "Merlin/Core/Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>

// TODO: i need to make this class multithreaded
//	-> lets try and put it in it's own thread first, such that it doesnt block the other processes
//	-> we would need to modify the other systems to support this (empty mesh rendering)

//	-> then we can try and figure out which parts below we can separate into threads

namespace Merlin {
	class ModelLoader {
	public:
		using MeshMaterialPair = std::pair<std::shared_ptr<MeshAsset>, Material>;

		static std::vector<MeshMaterialPair> Load(AssetManager& assets, const std::string& path) {
			std::vector<MeshMaterialPair> result;

			MERLIN_CORE_INFO("Loading model: {0}", path);

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs |
				aiProcess_CalcTangentSpace
			);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				MERLIN_CORE_ERROR("Failed to load model: {0}", importer.GetErrorString());
				return result;
			}

			MERLIN_CORE_INFO("Scene info: {0} meshes, {1} materials, {2} textures",
				scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

			std::string directory;
			size_t lastSlash = path.find_last_of("/\\");
			directory = (lastSlash != std::string::npos) ? path.substr(0, lastSlash + 1) : "";

			ProcessNode(assets, scene->mRootNode, scene, glm::mat4(1.0f), path, directory, result);

			MERLIN_CORE_INFO("Model loaded successfully: {0} meshes processed", result.size());
			return result;
		}

	private:
		static void ProcessNode(
			AssetManager& assets,
			aiNode* node,
			const aiScene* scene,
			const glm::mat4& parentTransform,
			const std::string& modelPath,
			const std::string& directory,
			std::vector<MeshMaterialPair>& result
		) {
			MERLIN_CORE_INFO("Processing node: {0} ({1} meshes, {2} children)",
				node->mName.C_Str(), node->mNumMeshes, node->mNumChildren);

			glm::mat4 nodeTransform = ConvertMatrix(node->mTransformation);
			glm::mat4 globalTransform = parentTransform * nodeTransform;

			for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				result.push_back(ProcessMesh(assets, mesh, scene, globalTransform, modelPath, directory, result.size()));
			}

			for (unsigned int i = 0; i < node->mNumChildren; ++i) {
				ProcessNode(assets, node->mChildren[i], scene, globalTransform, modelPath, directory, result);
			}
		}

		static MeshMaterialPair ProcessMesh(
			AssetManager& assets,
			aiMesh* mesh,
			const aiScene* scene,
			const glm::mat4& transform,
			const std::string& modelPath,
			const std::string& directory,
			size_t meshIndex
		) {
			MERLIN_CORE_INFO("Mesh[{0}]: {1} ({2} vertices, {3} faces)",
				meshIndex, mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			vertices.reserve(mesh->mNumVertices);

			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(transform)));

			for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
				Vertex vertex;

				glm::vec4 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
				vertex.position = glm::vec3(transform * pos);

				if (mesh->HasNormals()) {
					glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					vertex.normal = glm::normalize(normalMatrix * normal);
				}

				if (mesh->HasTangentsAndBitangents()) {
					glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
					glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
					vertex.tangent = glm::normalize(normalMatrix * tangent);
					vertex.bitangent = glm::normalize(normalMatrix * bitangent);
				}

				if (mesh->mTextureCoords[0]) {
					vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}

				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
				aiFace& face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j) {
					indices.push_back(face.mIndices[j]);
				}
			}

			std::string meshKey = modelPath + "#" + std::to_string(meshIndex);
			auto meshAsset = assets.Load<MeshAsset>(meshKey, std::move(vertices), std::move(indices));

			Material material;
			if (mesh->mMaterialIndex < scene->mNumMaterials) {
				aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

				aiColor4D color;
				if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
					material.albedo = glm::vec4(color.r, color.g, color.b, color.a);
				}

				float metallic, roughness;
				if (AI_SUCCESS == aiMat->Get(AI_MATKEY_METALLIC_FACTOR, metallic)) {
					material.metallic = metallic;
				}
				if (AI_SUCCESS == aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness)) {
					material.roughness = roughness;
				}

				material.albedoMap = LoadTexture(assets, aiMat, aiTextureType_DIFFUSE, directory, TextureType::ALBEDO);
				material.normalMap = LoadTexture(assets, aiMat, aiTextureType_NORMALS, directory, TextureType::NORMAL);
				material.metallicRoughnessMap = LoadTexture(assets, aiMat, aiTextureType_UNKNOWN, directory, TextureType::METALLIC_ROUGHNESS);
			}

			return { meshAsset, material };
		}

		static std::shared_ptr<TextureAsset> LoadTexture(
			AssetManager& assets,
			aiMaterial* mat,
			aiTextureType type,
			const std::string& directory,
			TextureType texType
		) {
			if (mat->GetTextureCount(type) == 0) return nullptr;

			aiString texPath;
			mat->GetTexture(type, 0, &texPath);

			std::string fullPath = directory + texPath.C_Str();
			MERLIN_CORE_INFO("Loading texture: {0}", fullPath);
			return assets.Load<TextureAsset>(fullPath, texType);
		}

		static glm::mat4 ConvertMatrix(const aiMatrix4x4& m) {
			return glm::mat4(
				m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4
			);
		}
	};
}
