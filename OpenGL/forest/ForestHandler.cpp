//
//  ForestHandler.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 15/12/21.
//

#include "ForestHandler.h"

ForestHandler::ForestHandler(){
    treeNumber = 1024;
    areaRadius = 15.0f; 
    
    srand((unsigned int)time(NULL));

    modelTree = new ModelMesh();
    ModelLoader::Load("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/models/tree.obj", *modelTree);

    for (int i = 0; i < treeNumber; i++) {
        treeAttributes.push_back(TreeAttribute{glm::vec3(), 1});
    }
    
    treeAttributesBuffer = new VertexBuffer(&treeAttributes[0], treeNumber * sizeof(TreeAttribute));
    treeAttributesBuffer->Bind();
    
    VertexBufferLayout instanceLayout;
    instanceLayout.AddFloat(3);
    instanceLayout.AddFloat(1);
    
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    modelTree->AddBufferLayout(*treeAttributesBuffer, instanceLayout);
    
//    treeAttributesPosition.BindUniformBlock(*ActiveShaders::TreeModelShader, "TreeAttributePosition", 0, treeNumber * 3 * sizeof(float));
//    treeAttributesSize.BindUniformBlock(*ActiveShaders::TreeModelShader, "TreeAttributeSize", 0, treeNumber * sizeof(glm::vec4));
}

ForestHandler::~ForestHandler(){
    delete modelTree;
    delete treeAttributesBuffer;
}

void ForestHandler::Update(Camera camera){
    glm::vec3 cameraPos = camera.GetPosition();

    std::uniform_real_distribution<float> distributionForX(0, areaRadius);
    std::uniform_real_distribution<float> distributionForY(0, areaRadius);
    std::uniform_real_distribution<float> distributionForZ(0, areaRadius);

    for (int i = 0; i < treeNumber; i++) {
        if (glm::distance(treeAttributes[i].position, cameraPos) > areaRadius){
            
            float xrand = distributionForX(generator);
            float yrand = distributionForY(generator);
            float zrand = distributionForZ(generator);
            
            glm::vec3 cameraPointOnSurface = glm::normalize(cameraPos + glm::vec3(xrand, yrand, zrand)) * (float)PlanetSettings::PlanetRadius;
            
            glm::vec3 pointHeight = MeshHeightHandler::GetHeight(cameraPointOnSurface.x,
                                                                 cameraPointOnSurface.y,
                                                                 cameraPointOnSurface.z,
                                                                 5) * glm::normalize(cameraPointOnSurface);
            glm::vec3 newTreePosition = cameraPointOnSurface + pointHeight;

            
            if (glm::distance(newTreePosition, cameraPos) < areaRadius &&
                glm::distance(newTreePosition, glm::vec3()) > PlanetSettings::SeaLevel) {
                treeAttributes[i].position = newTreePosition;
//                std::cout << camera.GetPosition().x << ", " << camera.GetPosition().y << ", " << camera.GetPosition().z << "\n";
//                treeAttributes.size[i] = ((float)(rand() % 5)) / 10 + 0.1;
                treeAttributes[i].size = 0.01;

            } else {
                treeAttributes[i].position = glm::vec3(0,0,0);
                treeAttributes[i].size = 1;
            }
        }
    }
    
    treeAttributesBuffer->Update(&treeAttributes[0], treeNumber * sizeof(TreeAttribute));
}

void ForestHandler::Render(){
    ActiveShaders::TreeModelShader->Bind();
    
//    ActiveShaders::TreeModelShader->SetUniform3fv("TreeAttributePosition", glm::value_ptr(treeAttributes[i].position[0]), treeNumber);
//    treeAttributesPosition.SetUniformBlock3fv("TreeAttributePosition", glm::value_ptr(treeAttributes.position[0]), treeNumber);
//    treeAttributesSize.SetUniformBlock4fv("TreeAttributeSize", glm::value_ptr(treeAttributes.position[0]), treeNumber);

    modelTree->Render(*ActiveShaders::TreeModelShader, treeNumber);
}
