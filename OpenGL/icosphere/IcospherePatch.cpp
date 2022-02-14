//
//  IcospherePatch.cpp
//  VulkanEngine
//
//  Created by Gabriele Padovani on 01/02/22.
//

#include "IcospherePatch.h"

IcospherePatch::IcospherePatch(glm::vec3 a, glm::vec3 b, glm::vec3 c, int maxSubs) {
    maxSubdivisionLevel = maxSubs;
    subdivideTriangle(a, b, c, 0);
    
    terrainPatchMesh = std::make_unique<ClipmapMesh>(vertices, indices); 
}


void IcospherePatch::addVertexAndIndex(glm::vec3 v){
    if (indexVertexPair.find(v) == indexVertexPair.end()){

        float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        v = v * sphereRadius / len;
        
        vertices.push_back(ClipmapVertex{v});
        indexVertexPair.emplace(v, vertexIndex);
        vertexIndex++;
    }
    indices.push_back(indexVertexPair[v]);
}

void IcospherePatch::subdivideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int subdivisionLevel){    
    if (maxSubdivisionLevel == subdivisionLevel){
        addVertexAndIndex(a);
        addVertexAndIndex(b);
        addVertexAndIndex(c);
    } else {
        
        glm::vec3 ab = (a + b) / 2.0f;
        glm::vec3 bc = (c + b) / 2.0f;
        glm::vec3 ac = (a + c) / 2.0f;
        
        subdivideTriangle(a, ab, ac, subdivisionLevel+1);
        subdivideTriangle(b, bc, ab, subdivisionLevel+1);
        subdivideTriangle(c, ac, bc, subdivisionLevel+1);
        subdivideTriangle(ab, bc, ac, subdivisionLevel+1);
    }
}


void IcospherePatch::Render(){
    //ActiveShaders::TerrainShader->SetUniform1f("u_TerrainAnimationPercentage", geomorphHandler.GetAnimationPercentage());
    terrainPatchMesh->Render(*ActiveShaders::ClipmapShader);
}
