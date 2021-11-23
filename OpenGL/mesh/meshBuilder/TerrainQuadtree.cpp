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
    
    if (!terrainPatch.has_value() && IsLeaf()){
        terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction);
    }
    
    if (NodeHasToMerge(camera)){
        Merge();
    }
    
    if (NodeHasToSplit(camera)){
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
        for (int i = 0; i < subdivisions.size(); i++) {
            subdivisions[i].Render(camera);
        }
    }
}

void TerrainQuadtree::Split(){
    terrainPatch = {};
    subdivisions.push_back(TerrainQuadtree(nodeX, nodeY, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX + nodeWidth / 2, nodeY, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1));
    subdivisions.push_back(TerrainQuadtree(nodeX + nodeWidth / 2, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1));
}

void TerrainQuadtree::Merge(){
    subdivisions = std::vector<TerrainQuadtree>();
    terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction);
}

bool TerrainQuadtree::IsVisible(Camera camera){
    return true || camera.PointIsVisibleFromCamera(nodeX, nodeY)
                || camera.PointIsVisibleFromCamera(nodeX + nodeWidth, nodeY)
                || camera.PointIsVisibleFromCamera(nodeX, nodeY + nodeWidth)
                || camera.PointIsVisibleFromCamera(nodeX + nodeWidth, nodeY + nodeWidth);
}

bool TerrainQuadtree::cameraIsCloseToTerrainPatch(glm::vec3 cameraPosition){
    float minDistance = QuadtreeSettings::MinDistanceToTerrain * nodeWidth;
    return glm::distance(cameraPosition, glm::vec3(GetCenter().x, 0, GetCenter().y)) < minDistance; 
}

bool TerrainQuadtree::NodeHasToMerge(Camera camera){
    return (!camera.PointIsVisibleFromCamera(GetCenter().x, GetCenter().y) ||
            !cameraIsCloseToTerrainPatch(camera.GetPosition())) && !IsLeaf();
}

bool TerrainQuadtree::NodeHasToSplit(Camera camera){
    return IsLeaf() &&
        IsVisible(camera) &&
        levelOfDetail < QuadtreeSettings::MaxSubdivisions &&
        cameraIsCloseToTerrainPatch(camera.GetPosition()); 
}

int TerrainQuadtree::GetVertexNumber(){
    if (IsLeaf()){
        return terrainPatch.value().GetVertexNumber();
    } else {
        int vertCount = 0;
        for (int i = 0; i < subdivisions.size(); i++) {
            vertCount += subdivisions[i].GetVertexNumber();
        }
        
        return vertCount; 
    }
}
