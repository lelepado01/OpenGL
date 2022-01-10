//
//  TriangleFace.h
//  OpenGL
//
//  Created by Gabriele Padovani on 07/01/22.
//

#ifndef TriangleFace_h
#define TriangleFace_h

#include <stdio.h>
#include <vector>
#include <optional>

#include "glm/glm.hpp"
#include "../camera/Camera.h"

class TriangleFace {
private:
    std::vector<TriangleFace> subdivisions = std::vector<TriangleFace>();

    glm::vec3 vertex1;
    glm::vec3 vertex2;
    glm::vec3 vertex3;
    
    int splitLevel; 
    float nodeWidth;
    glm::vec3 center;

private:
    void split();
    void merge();
    
    bool nodeHasToMerge(const Camera& camera) const;
    bool nodeHasToSplit(const Camera& camera) const;
    
    bool isLeaf() const;
    
    bool cameraIsCloseToTerrainPatch(const glm::vec3& cameraPosition) const;
    
public:
    TriangleFace(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int splitLevel);
    ~TriangleFace();
    
    bool Update(const Camera& camera);

    long GetVertexNumber() const ;
    
    std::vector<glm::vec3> GetVertices();
    
};


#endif /* TriangleFace_h */
