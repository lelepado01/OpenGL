//
//  TerrainQuadtree.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(int x, int y, TerrainFaceDirection dir) : root(nullptr) {
    nodeX = x;
    nodeY = y;
    nodeWidth = QuadtreeSettings::InitialWidth;
    distanceFromCamera = 1000;
    splitLevel = 1;
    direction = dir;
}

TerrainQuadtree::TerrainQuadtree(TerrainQuadtree* root, int x, int y, TerrainFaceDirection dir) : root(root) {
    nodeX = x;
    nodeY = y;
    nodeWidth = QuadtreeSettings::InitialWidth;
    distanceFromCamera = 1000;
    splitLevel = 1;
    direction = dir;
}

TerrainQuadtree::TerrainQuadtree(TerrainQuadtree* root, int x, int y, TerrainFaceDirection dir, int width, int LOD) : root(root) {
    nodeX = x;
    nodeY = y;
    nodeWidth = width;
    distanceFromCamera = 1000;
    splitLevel = LOD;
    direction = dir;
}

TerrainQuadtree::TerrainQuadtree(const TerrainQuadtree& terrainQuadtree) : root(terrainQuadtree.root) {
    copyData(terrainQuadtree);
}

TerrainQuadtree::TerrainQuadtree(TerrainQuadtree&& terrainQuadtree) : root(terrainQuadtree.root) {
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
        this->distanceFromCamera = terrainQuadtree.distanceFromCamera;
        this->splitLevel = terrainQuadtree.splitLevel; 
        this->direction = terrainQuadtree.direction;

        this->root = terrainQuadtree.root;
    }
}

TerrainQuadtree::~TerrainQuadtree(){
    
}

void TerrainQuadtree::Update(Camera camera){
    distanceFromCamera = glm::distance(camera.GetPosition(), getTerrainPatchCenter());

    if (!hasTerrain() && isLeaf()){
        int lod = LODHandler::GetLevelOfDetail(distanceFromCamera);
        terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, lod);
    }
    
    if (nodeHasToMerge(camera)){
        merge();
    }
    
    if (nodeHasToSplit(camera)){
        split();
    }
    
    if (isLeaf()){
        int lod = LODHandler::GetLevelOfDetail(distanceFromCamera);
        terrainPatch.value().Update(lod);
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
    TerrainQuadtree topLeft = TerrainQuadtree(root == nullptr ? this : root, nodeX, nodeY, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree topRight = TerrainQuadtree(root == nullptr ? this : root, nodeX + nodeWidth / 2, nodeY, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree bottomLeft = TerrainQuadtree(root == nullptr ? this : root, nodeX, nodeY + nodeWidth / 2, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree bottomRight = TerrainQuadtree(root == nullptr ? this : root, nodeX + nodeWidth / 2, nodeY + nodeWidth / 2, direction, nodeWidth / 2, splitLevel+1);
    
    subdivisions.push_back(topLeft);
    subdivisions.push_back(topRight);
    subdivisions.push_back(bottomLeft);
    subdivisions.push_back(bottomRight);
}

void TerrainQuadtree::merge(){
    int childSplitLevel = subdivisions[0].splitLevel;
    subdivisions = std::vector<TerrainQuadtree>();
    
    terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, childSplitLevel-1);
}

bool TerrainQuadtree::isVisible(const Camera& camera) const {
    
    if (!hasTerrain()) {
        return true;
    }

    glm::vec3 minPoint = terrainPatch.value().GetMinPoint();
    glm::vec3 maxPoint = terrainPatch.value().GetMaxPoint();

    return camera.RectAABBIsFrustumCulled(minPoint, maxPoint);
}

bool TerrainQuadtree::cameraIsCloseToTerrainPatch(const glm::vec3& cameraPosition) const {
    float minDistance = QuadtreeSettings::MinDistanceToTerrain * nodeWidth;
    return glm::distance(cameraPosition, getTerrainPatchCenter()) < minDistance; 
}

bool TerrainQuadtree::nodeHasToMerge(const Camera& camera) const {
    return !isLeaf() && !cameraIsCloseToTerrainPatch(camera.GetPosition());
}

bool TerrainQuadtree::nodeHasToSplit(const Camera& camera) const {
    return isLeaf()
        && isVisible(camera)
        && splitLevel < QuadtreeSettings::MaxSubdivisions
        && cameraIsCloseToTerrainPatch(camera.GetPosition());
}

glm::vec3 TerrainQuadtree::getTerrainPatchCenter() const {
    float centerX = nodeX + nodeWidth / 2;
    float centerZ = nodeY + nodeWidth / 2;
    
    glm::vec3 planePoint = glm::vec3(centerX, QuadtreeSettings::InitialWidth/2, centerZ);
    planePoint = TerrainPatch::PointCubeToSphere(TerrainFace::GetAxisRotationMatrix(direction) * planePoint);
    planePoint += MeshHeightHandler::GetApproximateHeight(planePoint.x, planePoint.y, planePoint.z) * glm::normalize(planePoint);
    
    return planePoint;
}


//TerrainQuadtree* TerrainQuadtree::neighbourLookup(TerrainQuadtree& terrain, TerrainFaceDirection direction){
//    if (direction == Top){
//        std::cout << terrain.nodeY << " - " << nodeY + nodeWidth << "\n";
//        if (terrain.nodeY == nodeY - nodeWidth){
//            if (terrain.nodeX < nodeX + nodeWidth && terrain.nodeX >= nodeX){
//                if (terrainPatch.has_value()){
//                    return this;
//                } else {
//                    if (terrain.nodeX < nodeX + nodeWidth / 2){
//                        return subdivisions.at(2).neighbourLookup(terrain, direction);
//                    } else {
//                        return subdivisions.at(3).neighbourLookup(terrain, direction);
//                    }
//                }
//            }
//        }
//    } else if (direction == Bottom){
//        if (terrain.nodeY + terrain.nodeWidth == nodeY){
//            if (terrain.nodeX < nodeX + nodeWidth && terrain.nodeX >= nodeX){
//                if (terrainPatch.has_value()){
//                    return this;
//                } else {
//                    if (terrain.nodeX < nodeX + nodeWidth / 2){
//                        return subdivisions.at(0).neighbourLookup(terrain, direction);
//                    } else {
//                        return subdivisions.at(1).neighbourLookup(terrain, direction);
//                    }
//                }
//            }
//        }
//    } else if (direction == Left){
//        if (terrain.nodeX == nodeX + nodeWidth){
//            if (terrain.nodeY < nodeY + nodeWidth && terrain.nodeY >= nodeY){
//                if (terrainPatch.has_value()){
//                    return this;
//                } else {
//                    if (terrain.nodeY < nodeY + nodeWidth / 2){
//                        return subdivisions.at(1).neighbourLookup(terrain, direction);
//                    } else {
//                        return subdivisions.at(3).neighbourLookup(terrain, direction);
//                    }
//                }
//            }
//        }
//    } else if (direction == Right){
//        if (terrain.nodeX + terrain.nodeWidth == nodeX){
//            if (terrain.nodeY < nodeY + nodeWidth && terrain.nodeY >= nodeY){
//                if (terrainPatch.has_value()){
//                    return this;
//                } else {
//                    if (terrain.nodeY < nodeY + nodeWidth / 2){
//                        return subdivisions.at(0).neighbourLookup(terrain, direction);
//                    } else {
//                        return subdivisions.at(2).neighbourLookup(terrain, direction);
//                    }
//                }
//            }
//        }
//    }
//    
//    return nullptr;
//}

long TerrainQuadtree::GetVertexNumber(const Camera& camera) const {
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
