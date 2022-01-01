//
//  ModelLoader.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 06/12/21.
//

#include "ModelLoader.h"

void ModelLoader::Load(const std::string& path, ModelMesh& mesh){

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
    
    if (!warn.empty()) {
        std::cout << "---\nWARNING: " << warn << "---\n" << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "---\nERROR: " << err << "---\n" << std::endl;
    }

    if (!ret) {
        exit(1);
    }
    
    for (const auto& shape : shapes) {
        unsigned int vertexIndex = 0;
        for (const auto& index : shape.mesh.indices) {

            glm::vec3 position {
                attrib.vertices[3 * size_t(index.vertex_index) + 0],
                attrib.vertices[3 * size_t(index.vertex_index) + 1],
                attrib.vertices[3 * size_t(index.vertex_index) + 2]
            };

            ModelVertex v = {};
            v.position = position;
            v.deltaPosition = glm::vec3(0.0f);

            if (index.normal_index >= 0) {
                tinyobj::real_t nx = attrib.normals[3*size_t(index.normal_index)+0];
                tinyobj::real_t ny = attrib.normals[3*size_t(index.normal_index)+1];
                tinyobj::real_t nz = attrib.normals[3*size_t(index.normal_index)+2];
                v.normal = glm::vec3(nx, ny, nz);
            }

            int materialId = shape.mesh.material_ids[(int)vertexIndex / 3];
            if (materialId < materials.size()){
                v.color = glm::vec3(materials[materialId].diffuse[0],
                                    materials[materialId].diffuse[1],
                                    materials[materialId].diffuse[2]);
            }
            
            mesh.AddVertex(v);
            mesh.AddIndex(vertexIndex);
            vertexIndex++;
        }
    }

    mesh.UpdateBuffers();
}


