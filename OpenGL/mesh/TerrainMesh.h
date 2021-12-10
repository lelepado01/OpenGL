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
    
    glm::vec3 minVertex;
    glm::vec3 maxVertex;
    
protected:
    void copyData(const TerrainMesh& mesh);
    void calculateMinMax(const glm::vec3& point);

    glm::vec3 computeVertexNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const;

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
    
    const glm::vec3& GetMinPoint() const { return minVertex; };
    const glm::vec3& GetMaxPoint() const { return maxVertex; };    
};

#endif /* TerrainMesh_h */
