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
    treeGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
    modelTree = new ModelMesh();
    ModelLoader::Load("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/models/tree.obj", *modelTree);

    for (int i = 0; i < treeNumber; i++) {
        treeAttributes.push_back(TreeAttribute{glm::vec3(),1, 0});
        treeModelMatrices.push_back(glm::mat4());
    }
    
    treeAttributesBuffer = new VertexBuffer(treeAttributes.data(), treeNumber * sizeof(TreeAttribute));
    treeAttributesBuffer->Bind();
    
    VertexBufferLayout instanceLayout;
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
            
            if (isSuitable(newTreePosition, cameraPos)) {
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

//void ForestHandler::Update(const Camera& camera){
//    glm::vec3 cameraPos = camera.GetPosition();
//
//    for (int i = 0; i < treeAttributes.size(); i++) {
//        if (glm::distance(treeAttributes[i].position, cameraPos) > areaRadius){
//            treeAttributes.erase(treeAttributes.begin() + i);
//            treeModelMatrices.erase(treeModelMatrices.begin() + i);
//        }
//    }
//
//    int treesToSpawn = treeNumber - (int)treeAttributes.size();
//
//    for (int i = 0; i < treesToSpawn; i++) {
//        glm::vec3 point = getPointAtEdgeOfVision(cameraPos);
//
////        if (treeGenerator.GetNoise(point.x, point.y, point.z) > 0.3) {
//
//            if (isSuitable(point, cameraPos)) {
//                TreeAttribute t {
//                    point,
//                    0.005f + ((float)(rand() % 4)) / 500.0f,
//                    (float)(rand() % 360)
//                };
//                treeAttributes.push_back(t);
//
//                glm::mat4 translate = glm::translate(glm::mat4(1.0f), t.position);
//                glm::vec3 center = glm::normalize(glm::abs(modelTree->GetCenter()));
//                glm::mat4 rotate =  glm::rotate(glm::mat4(1.0f), t.rotation, center);
//                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(t.size));
//                treeModelMatrices.push_back(translate * scale * rotate);
//
//            } else {
//
//            }
////        }
//    }
//
//    treeAttributesBuffer->Bind();
//    treeAttributesBuffer->Update(treeModelMatrices.data(), (unsigned int)treeModelMatrices.size() * sizeof(glm::mat4));
//}

void ForestHandler::Render(){
    ActiveShaders::TreeModelShader->Bind();
    modelTree->Render(*ActiveShaders::TreeModelShader, treeNumber);
}

bool ForestHandler::isSuitable(const glm::vec3 &pos, const glm::vec3 &cameraPosition){
    return glm::distance(pos, cameraPosition) < areaRadius && glm::distance(pos, glm::vec3()) > PlanetSettings::SeaLevel;
}


glm::vec3 ForestHandler::getPointAtEdgeOfVision(const glm::vec3 &camera){
    glm::vec3 up = camera;
    float randomAngle = 0.005f + ((float)(rand() % 4)) / 500.0f;
    
    glm::vec3 cameraOnSurface = glm::normalize(camera) * (float)PlanetSettings::PlanetRadius;

    glm::vec3 forward = glm::vec3(1, 1, -(cameraOnSurface.y + cameraOnSurface.x) / cameraOnSurface.z);
    forward = glm::normalize(forward) * (float)areaRadius;
    glm::vec3 areaSizeOnSurface = glm::normalize(camera + forward) * (float)PlanetSettings::PlanetRadius;
    
    return glm::rotate(areaSizeOnSurface, randomAngle, up);
}
