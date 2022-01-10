//
//  TriangleFace.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 07/01/22.
//

#include "TriangleFace.h"

TriangleFace::TriangleFace(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int splitLevel){
    vertex1 = v1;
    vertex2 = v2;
    vertex3 = v3;
    this->splitLevel = splitLevel; 
    
    nodeWidth = pow(glm::distance(v1, v2), 2);
    center = (v1 + v2 + v3) / 3.0f;
}

TriangleFace::~TriangleFace(){}

void TriangleFace::split(){
    float radius = 100;
    glm::vec3 nv1 = glm::normalize((vertex1 + vertex2) / 2.0f)*radius;
    glm::vec3 nv2 = glm::normalize((vertex2 + vertex3) / 2.0f)*radius;
    glm::vec3 nv3 = glm::normalize((vertex3 + vertex1) / 2.0f)*radius;

    subdivisions.push_back(TriangleFace(vertex1, nv1, nv3, splitLevel+1));
    subdivisions.push_back(TriangleFace(nv1, vertex2, nv2, splitLevel+1));
    subdivisions.push_back(TriangleFace(nv3, nv2, vertex3, splitLevel+1));
    subdivisions.push_back(TriangleFace(nv1, nv2, nv3, splitLevel+1));
}

void TriangleFace::merge(){
    subdivisions = std::vector<TriangleFace>();
}

bool TriangleFace::isLeaf() const {
    return subdivisions.size() == 0;
}

bool TriangleFace::Update(const Camera &camera){
    bool updated = false;
    
    if (nodeHasToMerge(camera)){
        merge();
        updated = true;
    }
    
    if (nodeHasToSplit(camera)){
        split();
        updated = true; 
    }
    
    if (!isLeaf()){
        for (int i = 0; i < subdivisions.size(); i++) {
            updated = updated || subdivisions[i].Update(camera);
        }
    }
    
    return updated;
}

bool TriangleFace::nodeHasToMerge(const Camera& camera) const {
    return !isLeaf() && !cameraIsCloseToTerrainPatch(camera.GetPosition()) && splitLevel >= 0;
}

bool TriangleFace::nodeHasToSplit(const Camera& camera) const {
    return isLeaf()
//        && isVisible(camera)
        && splitLevel < QuadtreeSettings::MaxSubdivisions
        && cameraIsCloseToTerrainPatch(camera.GetPosition());
}

bool TriangleFace::cameraIsCloseToTerrainPatch(const glm::vec3& cameraPosition) const {
    float distance = glm::distance(cameraPosition, center);
    return distance * (splitLevel+1) < nodeWidth*nodeWidth;
}


long TriangleFace::GetVertexNumber() const {
    return 12;
//    if (subdivisions.size() == 0 && mesh.has_value()) return mesh->GetVertexNumber();
//
//    long sum = 0;
//    for (int i = 0; i< subdivisions.size(); i++) {
//        sum += subdivisions[i].GetVertexNumber();
//    }
//
//    return sum;
}


std::vector<glm::vec3> TriangleFace::GetVertices(){
    std::vector<glm::vec3> verts = std::vector<glm::vec3>();

    if (isLeaf()){
        verts.push_back(vertex1);
        verts.push_back(vertex2);
        verts.push_back(vertex3);
    }  else {
        for (int i = 0; i < subdivisions.size(); i++) {
            std::vector<glm::vec3> v = subdivisions[i].GetVertices();
            verts.insert(verts.end(), v.begin(), v.end());
        }
    }
        
    return verts;
}
