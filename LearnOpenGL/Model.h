#ifndef __LearnOpenGL__Model__
#define __LearnOpenGL__Model__

#include "Mesh.h"
#include <iostream>

class Model
{
    
public:

    Model(GLchar* path) { this->loadModel(path); }
    void draw(GlslProgram &program);
    
private:

    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(const std::string &path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    
};
#endif
