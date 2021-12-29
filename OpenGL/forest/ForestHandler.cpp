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
        treeAttributes.push_back(TreeAttribute{glm::vec3(),1, 0});
        treeModelMatrices.push_back(glm::mat4());
    }
    
    treeAttributesBuffer = new VertexBuffer(treeAttributes.data(), treeNumber * sizeof(TreeAttribute));
    treeAttributesBuffer->Bind();
    
    VertexBufferLayout instanceLayout;
//    instanceLayout.AddFloat(3, true);
//    instanceLayout.AddFloat(1, true);
    instanceLayout.AddMat4x4(1, true);

    modelTree->AddBufferLayout(*treeAttributesBuffer, instanceLayout);
}

ForestHandler::~ForestHandler(){
    delete modelTree;
    delete treeAttributesBuffer;
}

void ForestHandler::Update(const Camera& camera){
    glm::vec3 cameraPos = camera.GetPosition();

    std::uniform_real_distribution<float> distributionForX(0, areaRadius);
    std::uniform_real_distribution<float> distributionForY(0, areaRadius);
    std::uniform_real_distribution<float> distributionForZ(0, areaRadius);

    for (int i = 0; i < treeNumber; i++) {
        if (glm::distance(treeAttributes[i].position, cameraPos) > areaRadius){
            
            srand((unsigned int)time(NULL));

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
                treeAttributes[i].size = 0.005 + ((float)(rand() % 4)) / 500;
                treeAttributes[i].rotation = (float)(rand() % 360);
                
                glm::mat4 translate = glm::translate(glm::mat4(1.0f), treeAttributes[i].position);
                glm::vec3 center = glm::normalize(glm::abs(modelTree->GetCenter()));
                glm::mat4 rotate =  glm::rotate(glm::mat4(1.0f), treeAttributes[i].rotation, center);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(treeAttributes[i].size));
                treeModelMatrices[i] = translate * scale * rotate;

            } else {
                treeAttributes[i].position = glm::vec3(0,0,0);
                treeAttributes[i].size = 1;
                treeAttributes[i].rotation = 0;
                treeModelMatrices[i] = glm::mat4(); 
            }
        }
    }
    
    treeAttributesBuffer->Bind(); 
    treeAttributesBuffer->Update(treeModelMatrices.data(), treeNumber * sizeof(glm::mat4));
}

void ForestHandler::Render(){
    ActiveShaders::TreeModelShader->Bind();
    modelTree->Render(*ActiveShaders::TreeModelShader, treeNumber);
}
