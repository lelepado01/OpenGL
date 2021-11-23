//
//  TerrainQuadtree.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(int x, int y, TerrainFaceDirection dir){
    nodeX = x;
    nodeY = y;
    nodeWidth = QuadtreeSettings::InitialWidth;
    levelOfDetail = 1;
    direction = dir;
}


TerrainQuadtree::TerrainQuadtree(int x, int y, TerrainFaceDirection dir, int width, int LOD){
    nodeX = x;
    nodeY = y;
    nodeWidth = width;
    levelOfDetail = LOD;
    direction = dir;
}

TerrainQuadtree::~TerrainQuadtree(){}

void TerrainQuadtree::Update(Camera camera){
    
    if (!terrainPatch.has_value() && isLeaf()){
        terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction);
    }
    
    if (nodeHasToMerge(camera)){
        merge();
    }
    
    if (nodeHasToSplit(camera)){
        split();
    }
    
    if (!isLeaf()) {
        for (int i = 0; i < subdivisions.size(); i++) {
            subdivisions[i].Update(camera);
        }
    }
}

void TerrainQuadtree::Render(Camera camera){
    if (!isVisible(camera)){
        return; 
    }
    
    if (isLeaf()){
        terrainPatch.value().Render();
    } else {
        for (int i = 0; i < subdivisions.size(); i++) {
            subdivisions[i].Render(camera);
        }
    }
}

void TerrainQuadtree::split(){
    terrainPatch = {};
    subdivisions.push_back(TerrainQuadtree(nodeX, nodeY, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX + nodeWidth / 2, nodeY, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX + nodeWidth / 2, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1));
}

void TerrainQuadtree::merge(){
    subdivisions = std::vector<TerrainQuadtree>();
    terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction);
}

bool TerrainQuadtree::isVisible(Camera camera){
    return true || camera.PointIsVisibleFromCamera(nodeX, nodeY)
                || camera.PointIsVisibleFromCamera(nodeX + nodeWidth, nodeY)
                || camera.PointIsVisibleFromCamera(nodeX, nodeY + nodeWidth)
                || camera.PointIsVisibleFromCamera(nodeX + nodeWidth, nodeY + nodeWidth);
}

bool TerrainQuadtree::cameraIsCloseToTerrainPatch(glm::vec3 cameraPosition){
    float minDistance = QuadtreeSettings::MinDistanceToTerrain * nodeWidth;
    return glm::distance(cameraPosition, getTerrainPatchCenter()) < minDistance; 
}

bool TerrainQuadtree::nodeHasToMerge(Camera camera){
    return !isLeaf() && !cameraIsCloseToTerrainPatch(camera.GetPosition());
}

bool TerrainQuadtree::nodeHasToSplit(Camera camera){
    return isLeaf()
        && isVisible(camera)
        && levelOfDetail < QuadtreeSettings::MaxSubdivisions
        && cameraIsCloseToTerrainPatch(camera.GetPosition());
}

int TerrainQuadtree::GetVertexNumber(){
    if (isLeaf()){
        return terrainPatch.value().GetVertexNumber();
    } else {
        int vertCount = 0;
        for (int i = 0; i < subdivisions.size(); i++) {
            vertCount += subdivisions[i].GetVertexNumber();
        }
        
        return vertCount; 
    }
}

glm::vec3 TerrainQuadtree::getTerrainPatchCenter(){
    float centerX = nodeX + nodeWidth / 2;
    float centerZ = nodeY + nodeWidth / 2;
    glm::vec3 planePoint = glm::vec3(centerX,
                                     QuadtreeSettings::InitialWidth / 2 + MeshHeight::GetHeight(centerX, centerZ),
                                     centerZ);
    return TerrainPatch::PointCubeToSphere(TerrainFace::GetAxisRotationMatrix(direction) * planePoint);
}

//Vertex v = {};
//v.position = glm::vec3(globalX, MeshHeight::GetHeight(globalX ,globalZ) + globalPositionY, globalZ);
//v.position = axisRotationMatrix * v.position;
//v.position = pointCubeToSphere(v.position);

