//
//  TerrainQuadtree.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(int x, int y, TerrainFaceDirection dir) {
    nodeX = x;
    nodeY = y;
    nodeWidth = QuadtreeSettings::InitialWidth;
    distanceFromCamera = 1000;
    splitLevel = 1;
    direction = dir;
}

TerrainQuadtree::TerrainQuadtree(int x, int y, TerrainFaceDirection dir, int width, int LOD) {
    nodeX = x;
    nodeY = y;
    nodeWidth = width;
    distanceFromCamera = 1000;
    splitLevel = LOD;
    direction = dir;
}

TerrainQuadtree::TerrainQuadtree(const TerrainQuadtree& terrainQuadtree) {
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
        this->distanceFromCamera = terrainQuadtree.distanceFromCamera;
        this->splitLevel = terrainQuadtree.splitLevel; 
        this->direction = terrainQuadtree.direction;
    }
}

TerrainQuadtree::~TerrainQuadtree(){
    
}

void TerrainQuadtree::Update(Camera camera){
    distanceFromCamera = glm::distance(camera.GetPosition(), getTerrainPatchCenter());

    if (!hasTerrain() && isLeaf()){
        int lod = LODHandler::GetLevelOfDetail(distanceFromCamera);
        terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, lod, TerrainPatchAnimation::Upscale);
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
    TerrainQuadtree topLeft = TerrainQuadtree(nodeX, nodeY, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree topRight = TerrainQuadtree(nodeX + nodeWidth / 2, nodeY, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree bottomLeft = TerrainQuadtree(nodeX, nodeY + nodeWidth / 2, direction, nodeWidth / 2, splitLevel+1);
    TerrainQuadtree bottomRight = TerrainQuadtree(nodeX + nodeWidth / 2, nodeY + nodeWidth / 2, direction, nodeWidth / 2, splitLevel+1);
    
    subdivisions.push_back(topLeft);
    subdivisions.push_back(topRight);
    subdivisions.push_back(bottomLeft);
    subdivisions.push_back(bottomRight);
}

void TerrainQuadtree::merge(){
    int childSplitLevel = subdivisions[0].splitLevel;
    subdivisions = std::vector<TerrainQuadtree>();
    
    terrainPatch = TerrainPatch(nodeX, nodeY, nodeWidth, direction, childSplitLevel-1, TerrainPatchAnimation::Downscale);
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
    planePoint += MeshHeightHandler::GetHeight(planePoint.x, planePoint.y, planePoint.z, 2) * glm::normalize(planePoint);
    
    return planePoint;
}

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
