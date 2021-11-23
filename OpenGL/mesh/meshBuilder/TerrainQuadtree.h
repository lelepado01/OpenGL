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
    bool cameraIsCloseToTerrainPatch(glm::vec3 cameraPosition);
    
public:
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir);
    TerrainQuadtree(int x, int y, TerrainFaceDirection dir, int width, int LOD);
    ~TerrainQuadtree();
    
    void Split();
    void Merge();
    
    void Update(Camera camera);
    void Render(Camera camera);
    
    int GetVertexNumber(); 

    bool IsLeaf() { return subdivisions.size() == 0; };
    bool IsVisible(Camera camera);
    bool NodeHasToMerge(Camera camera);
    bool NodeHasToSplit(Camera camera);
    
    glm::vec3 GetTerrainPatchCenter();

};

#endif /* TerrainQuadtree_h */
