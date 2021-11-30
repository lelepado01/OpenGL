//
//  PlanetaryMesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 22/11/21.
//

#ifndef PlanetaryMesh_h
#define PlanetaryMesh_h

#include <stdio.h>
#include <vector>

#include "TerrainQuadtree.h"

class PlanetaryMesh {
private:
    std::vector<TerrainQuadtree> faces = std::vector<TerrainQuadtree>();
    
public:
    PlanetaryMesh();
    
    void Update(Camera camera);
    void Render(Camera camera);
    
    int GetVertexNumber(Camera camera); 
};

#endif /* PlanetaryMesh_h */
