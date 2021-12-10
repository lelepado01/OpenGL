//
//  TerrainMesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 09/12/21.
//

#ifndef TerrainMesh_h
#define TerrainMesh_h

#include <stdio.h>
#include "Mesh.h"

class TerrainMesh : Mesh {
private:
    
    std::vector<TerrainVertex> vertices = std::vector<TerrainVertex>();
    
public:
    
};

#endif /* TerrainMesh_h */
