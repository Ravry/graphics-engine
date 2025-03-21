#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct BoneInfo {
    glm::mat4 boneOffset;
    glm::mat4 finalTransformation;
};

struct VertexBoneData {
    unsigned int IDs[4];
    float weights[4];

    VertexBoneData() {
        memset(IDs, 0, sizeof(IDs));
        memset(weights, 0, sizeof(weights));
    }

    void AddBoneData(unsigned int BoneID, float Weight) {
        for (int i = 0; i < 4; i++) {
            if (weights[i] == 0.0f) {
                IDs[i] = BoneID;
                weights[i] = Weight;
                return;
            }
        }
        // std::cerr << "WARNING: More than 4 bone influences per vertex, extra weights discarded!" << std::endl;
    }
};

struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<VertexBoneData> bones;
};

class Model {
private:
    std::vector<BoneInfo> boneInfo;
    std::unordered_map<std::string, unsigned int> boneMapping;
    unsigned int numBones = 0;

    void processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<VertexBoneData> bones(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            if (mesh->mTextureCoords[0]) {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if (mesh->HasNormals()) {
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);      
            }
        }

        loadBones(mesh, bones);

        std::cout << "MODEL::BONES::COUNT" << bones.size() << std::endl;

        return {vertices, indices, bones};
    }

    void loadBones(aiMesh* mesh, std::vector<VertexBoneData>& bones) {
        for (unsigned int i = 0; i < mesh->mNumBones; i++) {
            std::string boneName = mesh->mBones[i]->mName.data;
            unsigned int boneIndex = 0;

            if (boneMapping.find(boneName) == boneMapping.end()) {
                boneIndex = numBones++;
                boneInfo.push_back({glm::transpose(glm::make_mat4(&mesh->mBones[i]->mOffsetMatrix.a1))});
                boneMapping[boneName] = boneIndex;
            } else {
                boneIndex = boneMapping[boneName];
            }

            for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
                unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;
                bones[vertexID].AddBoneData(boneIndex, weight);
            }
        }
    }

public: 
    std::vector<Mesh> meshes;

    Model(const char* modelfile) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelfile, aiProcess_Triangulate | 
                                                             aiProcess_GenSmoothNormals | 
                                                             aiProcess_FlipUVs | 
                                                             aiProcess_CalcTangentSpace);
        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        processNode(scene->mRootNode, scene);
    }
};