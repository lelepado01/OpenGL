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

#include "../../camera/Camera.h"
#include "TerrainQuadtree.h"

class PlanetaryMesh {
private:
    std::vector<TerrainQuadtree> faces = std::vector<TerrainQuadtree>();
    
public:
    PlanetaryMesh();
    ~PlanetaryMesh();
    
    void Update(Camera camera);
    void Render(Camera camera);
    
    int GetVertexNumber(); 
};

#endif /* PlanetaryMesh_h */
