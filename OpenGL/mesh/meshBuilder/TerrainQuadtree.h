//
//  TerrainQuadtree.h
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef TerrainQuadtree_h
#define TerrainQuadtree_h

#include <stdio.h>
#include <vector>
#include <optional>

#include "../../camera/Camera.h"
#include "../../settings/QuadtreeSettings.h"
#include "TerrainPatch.h"

#include <glm/gtx/string_cast.hpp>

class TerrainQuadtree {
private:
    int nodeX, nodeY, nodeWidth;
    int levelOfDetail;
    TerrainFaceDirection direction;
    
    std::unordered_map<TerrainFaceDirection, std::vector<TerrainQuadtree*>> neighbours = std::unordered_map<TerrainFaceDirection, std::vector<TerrainQuadtree*>>(); 

    std::vector<TerrainQuadtree> subdivisions = std::vector<TerrainQuadtree>();
    std::optional<TerrainPatch> terrainPatch = {};

private:
    void copyData(const TerrainQuadtree& terrainQuadtree);
    void split();
    void merge();
    
    bool isVisible(const Camera& camera) const;
    bool isLeaf() const { return subdivisions.size() == 0; };
    
    bool nodeHasToMerge(const Camera& camera);
    bool nodeHasToSplit(const Camera& camera);
    bool cameraIsCloseToTerrainPatch(const glm::vec3& cameraPosition);
    
    glm::vec3 getTerrainPatchCenter();
    TerrainQuadtree* getCorrectNeighbourFor(TerrainQuadtree& terrain, TerrainFaceDirection dir); 
    
//    TerrainQuadtree* getSubdivisionNeighbour(TerrainFaceDirection direction);
    
public:
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir);
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir, int width, int LOD);
    TerrainQuadtree(const TerrainQuadtree& terrainQuadtree);
    TerrainQuadtree(TerrainQuadtree&& terrainQuadtree);
    ~TerrainQuadtree();
    TerrainQuadtree& operator=(const TerrainQuadtree& terrainQuadtree);

    void Update(Camera camera);
    void Render(Camera camera);
    
    int GetVertexNumber(const Camera& camera) const;
    
    void SetNeighbour(TerrainQuadtree* neigh, TerrainFaceDirection direction);
    void PairNeighbour(TerrainQuadtree* neigh, TerrainFaceDirection direction);
    void RemoveNeighbour(TerrainQuadtree *neigh, TerrainFaceDirection direction); 

};

//struct NeighbourData {
//    std::vector<TerrainQuadtree&> leftNeighs;
//    std::vector<TerrainQuadtree&> rightNeighs;
//    std::vector<TerrainQuadtree&> topNeighs;
//    std::vector<TerrainQuadtree&> bottomNeighs;
//};

#endif /* TerrainQuadtree_h */
