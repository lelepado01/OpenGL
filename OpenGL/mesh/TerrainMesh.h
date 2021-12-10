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

class TerrainMesh : public Mesh {
private:
    std::vector<TerrainVertex> vertices = std::vector<TerrainVertex>();
        
protected:
    void copyData(const TerrainMesh& mesh);

public:
    TerrainMesh();
    ~TerrainMesh();
    TerrainMesh(const TerrainMesh& mesh);
    TerrainMesh(TerrainMesh&& mesh);
    TerrainMesh& operator=(const TerrainMesh& mesh);
    
    void UpdateBuffers();
    void Clear();
    void RecalculateNormals();
    
    virtual void AddVertex(TerrainVertex v);    
};

#endif /* TerrainMesh_h */
