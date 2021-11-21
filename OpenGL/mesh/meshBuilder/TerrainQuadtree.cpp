//
//  TerrainQuadtree.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainQuadtree.h"


TerrainQuadtree::TerrainQuadtree(int x, int y, int width, int LOD){
    this->x = x;
    this->y = y;
    this->width = width;
    this->LOD = LOD;
}

TerrainQuadtree::~TerrainQuadtree(){}

void TerrainQuadtree::Update(Camera camera){
    
    if (!terrainPatch.has_value() && IsLeaf()){
        terrainPatch = TerrainPatch(x, y, width);
    }
    
    if ((!IsVisible(camera) || !cameraIsCloseToTerrainPatch(camera.GetPosition())) && !IsLeaf()){
        Merge();
    }
    
    if (IsLeaf() && IsVisible(camera) && LOD < QuadtreeSettings::MaxSubdivisions && cameraIsCloseToTerrainPatch(camera.GetPosition())){
        Split();
    }
    
    if (!IsLeaf()) {
        for (int i = 0; i < subdivisions.size(); i++) {
            subdivisions[i].Update(camera);
        }
    }
}

void TerrainQuadtree::Render(Camera camera){
    if (!IsVisible(camera)){
        return; 
    }
    
    if (IsLeaf()){
        terrainPatch.value().Render();
    } else {
        for (TerrainQuadtree children : subdivisions) {
            children.Render(camera);
        }
    }
}

void TerrainQuadtree::Split(){
    terrainPatch = {};
    subdivisions.push_back(TerrainQuadtree(x, y, width / 2, LOD+1));
    subdivisions.push_back(TerrainQuadtree(x + width / 2, y, width / 2, LOD+1));
    subdivisions.push_back(TerrainQuadtree(x, y + width / 2, width / 2, LOD+1));
    subdivisions.push_back(TerrainQuadtree(x + width / 2, y + width / 2, width / 2, LOD+1));
}

void TerrainQuadtree::Merge(){
    subdivisions = std::vector<TerrainQuadtree>();
    terrainPatch = TerrainPatch(x, y, width);
}

bool TerrainQuadtree::IsVisible(Camera camera){
    return true;
}

bool TerrainQuadtree::cameraIsCloseToTerrainPatch(glm::vec3 cameraPosition){
    int minDistance = 2 * width;
    return glm::distance(cameraPosition, glm::vec3(GetCenter().x, 0, GetCenter().y)) < minDistance; 
}

int TerrainQuadtree::GetVertexNumber(){
    if (IsLeaf()){
        return terrainPatch.value().GetVertexNumber();
    } else {
        int vertCount = 0;
        for (TerrainQuadtree children : subdivisions) {
            vertCount += children.GetVertexNumber();
        }
        
        return vertCount; 
    }
}
