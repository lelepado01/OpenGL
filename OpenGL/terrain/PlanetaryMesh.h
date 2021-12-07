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
    
    void Update(const Camera& camera);
    void Render(const Camera& camera);
    
    long GetVertexNumber(const Camera& camera) const;
};

#endif /* PlanetaryMesh_h */
