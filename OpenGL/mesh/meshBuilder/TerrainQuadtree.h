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

class TerrainQuadtree {
private:
    int nodeX, nodeY, nodeWidth;
    int levelOfDetail;
    TerrainFaceDirection direction; 
    
    std::vector<TerrainQuadtree> subdivisions = std::vector<TerrainQuadtree>();
    std::optional<TerrainPatch> terrainPatch = {};

private:
    void split();
    void merge();
    
    bool isVisible(Camera camera);
    bool isLeaf() { return subdivisions.size() == 0; };
    bool nodeHasToMerge(Camera camera);
    bool nodeHasToSplit(Camera camera);
    bool cameraIsCloseToTerrainPatch(glm::vec3 cameraPosition);
    
    glm::vec3 getTerrainPatchCenter();

public:
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir);
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir, int width, int LOD);
    
    void Update(Camera camera);
    void Render(Camera camera);
    
    int GetVertexNumber(); 

};

#endif /* TerrainQuadtree_h */
