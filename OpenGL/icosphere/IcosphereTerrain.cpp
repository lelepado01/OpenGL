//
//  IcosphereTerrain.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 07/01/22.
//

#include "IcosphereTerrain.h"
#include "../shaders/ActiveShaders.h"

IcosphereTerrain::IcosphereTerrain(){
    const float radius = 100;
    
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    glm::vec3 top = glm::vec3(0,0,radius);

    std::vector<glm::vec3> topRow = std::vector<glm::vec3>();
    std::vector<glm::vec3> bottomRow = std::vector<glm::vec3>();

    float xy = 0;
    float z = 0;
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;                // start from -90 deg at 2nd row
    
    for (int i = 0; i < 5; i++) {
        z  = radius * sinf(V_ANGLE);            // elevaton
        xy = radius * cosf(V_ANGLE);            // length on XY plane

        topRow.push_back(glm::vec3(xy * cosf(hAngle1), xy * sinf(hAngle1), z));
        bottomRow.push_back(glm::vec3(xy * cosf(hAngle2), xy * sinf(hAngle2), -z));
        
        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }
    
    glm::vec3 bottom = glm::vec3(0,0,-radius);
    
    faces.push_back(TriangleFace(top, topRow[0], topRow[1], 0));
    faces.push_back(TriangleFace(top, topRow[1], topRow[2], 0));
    faces.push_back(TriangleFace(top, topRow[2], topRow[3], 0));
    faces.push_back(TriangleFace(top, topRow[3], topRow[4], 0));
    faces.push_back(TriangleFace(top, topRow[4], topRow[0], 0));
    
    faces.push_back(TriangleFace(bottom, bottomRow[1], bottomRow[0], 0));
    faces.push_back(TriangleFace(bottom, bottomRow[2], bottomRow[1], 0));
    faces.push_back(TriangleFace(bottom, bottomRow[3], bottomRow[2], 0));
    faces.push_back(TriangleFace(bottom, bottomRow[4], bottomRow[3], 0));
    faces.push_back(TriangleFace(bottom, bottomRow[0], bottomRow[4], 0));
    
    faces.push_back(TriangleFace(topRow[0], bottomRow[0], topRow[1], 0));
    faces.push_back(TriangleFace(topRow[1], bottomRow[1], topRow[2], 0));
    faces.push_back(TriangleFace(topRow[2], bottomRow[2], topRow[3], 0));
    faces.push_back(TriangleFace(topRow[3], bottomRow[3], topRow[4], 0));
    faces.push_back(TriangleFace(topRow[4], bottomRow[4], topRow[0], 0));
    
    faces.push_back(TriangleFace(bottomRow[0], bottomRow[1], topRow[1], 0));
    faces.push_back(TriangleFace(bottomRow[1], bottomRow[2], topRow[2], 0));
    faces.push_back(TriangleFace(bottomRow[2], bottomRow[3], topRow[3], 0));
    faces.push_back(TriangleFace(bottomRow[3], bottomRow[4], topRow[4], 0));
    faces.push_back(TriangleFace(bottomRow[4], bottomRow[0], topRow[0], 0));

    for (int i = 0; i < faces.size(); i++) {
        meshes.push_back(TerrainMesh()); 
    }
}

void IcosphereTerrain::Update(const Camera &camera){
    
    for (int i = 0; i < faces.size(); i++) {
        
        bool faceHasUpdated = faces.at(i).Update(camera);
        
        if (faceHasUpdated){
            meshes.at(i).Clear();
            
            int globalIndex = 0;

            std::vector<glm::vec3> v = faces.at(i).GetVertices();
            for (int j = 0; j < v.size(); j++) {
                meshes.at(i).AddVertex(TerrainVertex{v[j], glm::vec3(), glm::vec3()});
                meshes.at(i).AddIndex(globalIndex);
                globalIndex++;
            }
            
            meshes.at(i).UpdateBuffers();
        }
    }
}

void IcosphereTerrain::Render(const Camera &camera){
    for (int i = 0; i < faces.size(); i++) {
        meshes.at(i).Render(*ActiveShaders::SimpleShader);
    }
}


long IcosphereTerrain::GetVertexNumber(const Camera &camera) const {
    long sum = 0;
    for (int i = 0; i < faces.size(); i++) {
        sum += faces.at(i).GetVertexNumber();
    }
    
    return sum; 
}
