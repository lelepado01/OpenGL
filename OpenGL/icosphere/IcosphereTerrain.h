//
//  IcosphereTerrain.h
//  OpenGL
//
//  Created by Gabriele Padovani on 07/01/22.
//

#ifndef IcosphereTerrain_h
#define IcosphereTerrain_h

#include <stdio.h>

#include "TriangleFace.h"
#include "../mesh/TerrainMesh.h"

class IcosphereTerrain {
    std::vector<TriangleFace> faces = std::vector<TriangleFace>();
    std::vector<TerrainMesh> meshes = std::vector<TerrainMesh>();
    
public:
    IcosphereTerrain();
    
    void Update(const Camera& camera);
    void Render(const Camera& camera);
    
    long GetVertexNumber(const Camera& camera) const;
};

#endif /* IcosphereTerrain_h */
