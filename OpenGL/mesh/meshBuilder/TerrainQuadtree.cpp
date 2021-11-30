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

TerrainQuadtree::TerrainQuadtree(const TerrainQuadtree& terrainQuadtree){
    copyData(terrainQuadtree);
}

TerrainQuadtree::TerrainQuadtree(TerrainQuadtree&& terrainQuadtree){
    copyData(terrainQuadtree);
}

TerrainQuadtree& TerrainQuadtree::operator=(const TerrainQuadtree& terrainQuadtree){
    copyData(terrainQuadtree);
    return *this;
}

void TerrainQuadtree::copyData(const TerrainQuadtree& terrainQuadtree){
    if (this != &terrainQuadtree) {
        this->nodeX = terrainQuadtree.nodeX;
        this->nodeY = terrainQuadtree.nodeY;
        this->nodeWidth = terrainQuadtree.nodeWidth;
        this->levelOfDetail = terrainQuadtree.levelOfDetail;
        this->direction = terrainQuadtree.direction;

        this->neighbours = terrainQuadtree.neighbours;
    }
}

TerrainQuadtree::~TerrainQuadtree(){
//    if (neighbours[Left] != nullptr) neighbours[Left]->SetNeighbour(nullptr, Right);
//    if (neighbours[Right] != nullptr) neighbours[Right]->SetNeighbour(nullptr, Left);
//    if (neighbours[Top] != nullptr) neighbours[Top]->SetNeighbour(nullptr, Bottom);
//    if (neighbours[Bottom] != nullptr) neighbours[Bottom]->SetNeighbour(nullptr, Top);
}

void TerrainQuadtree::Update(Camera camera){
    
    if (!terrainPatch.has_value() && isLeaf()){
        terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, levelOfDetail);
    }
    
    if (nodeHasToMerge(camera)){
        merge();
    }
    
    if (nodeHasToSplit(camera)){
        split();
    }
    
    if (isLeaf()){
        terrainPatch.value().Update();
    } else {
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
    TerrainQuadtree topLeft = TerrainQuadtree(nodeX, nodeY, direction, nodeWidth / 2, levelOfDetail+1);
    TerrainQuadtree topRight = TerrainQuadtree(nodeX + nodeWidth / 2, nodeY, direction, nodeWidth / 2, levelOfDetail+1);
    TerrainQuadtree bottomLeft = TerrainQuadtree(nodeX, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1);
    TerrainQuadtree bottomRight = TerrainQuadtree(nodeX + nodeWidth / 2, nodeY + nodeWidth / 2, direction, nodeWidth / 2, levelOfDetail+1);
    
//    topLeft.PairNeighbour(&topRight, Right);
//    topLeft.PairNeighbour(&bottomLeft, Bottom);
//    bottomRight.PairNeighbour(&topRight, Top);
//    bottomRight.PairNeighbour(&bottomLeft, Left);
//
//    topRight.PairNeighbour(getCorrectNeighbourFor(topRight, Right), Right);
//    bottomRight.PairNeighbour(getCorrectNeighbourFor(bottomRight, Right), Right);
//
//    topLeft.PairNeighbour(getCorrectNeighbourFor(topLeft, Top), Top);
//    topRight.PairNeighbour(getCorrectNeighbourFor(topRight, Top), Top);
//
//    topLeft.PairNeighbour(getCorrectNeighbourFor(topLeft, Left), Left);
//    bottomLeft.PairNeighbour(getCorrectNeighbourFor(bottomLeft, Left), Left);
//
//    bottomLeft.PairNeighbour(getCorrectNeighbourFor(bottomLeft, Bottom), Bottom);
//    bottomRight.PairNeighbour(getCorrectNeighbourFor(bottomRight, Bottom), Bottom);
    
    subdivisions.push_back(topLeft);
    subdivisions.push_back(topRight);
    subdivisions.push_back(bottomLeft);
    subdivisions.push_back(bottomRight);
}

void TerrainQuadtree::merge(){
    
//    PairNeighbour(getSubdivisionNeighbour(Right), Right);
//    PairNeighbour(getSubdivisionNeighbour(Left), Left);
//    PairNeighbour(getSubdivisionNeighbour(Top), Top);
//    PairNeighbour(getSubdivisionNeighbour(Bottom), Bottom);
    
    subdivisions = std::vector<TerrainQuadtree>();
    terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, levelOfDetail);
}

bool TerrainQuadtree::isVisible(Camera camera){
    
    if (!terrainPatch.has_value()) return true;
    
    glm::vec3 minPoint = terrainPatch.value().GetMinPoint();
    glm::vec3 maxPoint = terrainPatch.value().GetMaxPoint();

    return camera.PointIsVisibleFromCamera(minPoint, maxPoint);
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

int TerrainQuadtree::GetVertexNumber(Camera camera){
    if (isLeaf()){
        
        if (isVisible(camera)){
            return terrainPatch.value().GetVertexNumber();
        } else {
            return 0;
        }
        
    } else {
        
        int vertCount = 0;
        for (int i = 0; i < subdivisions.size(); i++) {
            vertCount += subdivisions[i].GetVertexNumber(camera);
        }
        
        return vertCount; 
    }
}

glm::vec3 TerrainQuadtree::getTerrainPatchCenter(){
    float centerX = nodeX + nodeWidth / 2;
    float centerZ = nodeY + nodeWidth / 2;
    
    glm::vec3 planePoint = glm::vec3(centerX, QuadtreeSettings::InitialWidth/2, centerZ);
    planePoint = TerrainPatch::PointCubeToSphere(TerrainFace::GetAxisRotationMatrix(direction) * planePoint);
    planePoint += MeshHeight::GetApproximateHeight(planePoint.x, planePoint.y, planePoint.z) * glm::normalize(planePoint);
    
    return planePoint;
}


//TerrainQuadtree* TerrainQuadtree::getSubdivisionNeighbour(TerrainFaceDirection direction){
//    for (int i = 0; i < subdivisions.size(); i++) {
//        if (direction == Left && subdivisions[i].nodeX == nodeX){
//            return subdivisions[i].neighbours[Left].get();
//        }
//        if (direction == Right && subdivisions[i].nodeX > nodeX){
//            return subdivisions[i].neighbours[Right].get();
//        }
//        if (direction == Top && subdivisions[i].nodeY == nodeY){
//            return subdivisions[i].neighbours[Top].get();
//        }
//        if (direction == Bottom && subdivisions[i].nodeY > nodeY){
//            return subdivisions[i].neighbours[Bottom].get();
//        }
//    }
//
//    return nullptr;
//}


TerrainQuadtree* TerrainQuadtree::getCorrectNeighbourFor(TerrainQuadtree &terrain, TerrainFaceDirection dir){
//    for (int i = 0; i < neighbours[dir].size(); i++) {
//        if (neighbours[dir][i]->)
//    }
    return nullptr;
}

void TerrainQuadtree::SetNeighbour(TerrainQuadtree *neigh, TerrainFaceDirection direction){
    neighbours[direction].push_back(neigh);
}

void TerrainQuadtree::RemoveNeighbour(TerrainQuadtree *neigh, TerrainFaceDirection direction){
    for (int i = 0; neighbours[direction].size(); i++) {
        if (neighbours[direction][i]->nodeX == neigh->nodeX && neighbours[direction][i]->nodeY == neigh->nodeY){
            neighbours[direction].erase(neighbours[direction].begin() + i); 
        }
    }
}

void TerrainQuadtree::PairNeighbour(TerrainQuadtree *neigh, TerrainFaceDirection direction){
    if (neigh != nullptr){
        if (direction == TerrainFaceDirection::Left){
            neigh->SetNeighbour(this, Right);
        }
        if (direction == TerrainFaceDirection::Right){
            neigh->SetNeighbour(this, Left);
        }
        if (direction == TerrainFaceDirection::Top){
            neigh->SetNeighbour(this, Bottom);
        }
        if (direction == TerrainFaceDirection::Bottom){
            neigh->SetNeighbour(this, Top);
        }

        neighbours[direction].push_back(neigh);
    }
}
