//
//  PlanetaryMesh.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 22/11/21.
//

#include "PlanetaryMesh.h"

PlanetaryMesh::PlanetaryMesh(){
    for (int i = 0; i < 6; i++) {
        faces.push_back(TerrainQuadtree(-QuadtreeSettings::InitialWidth / 2,-QuadtreeSettings::InitialWidth / 2, (TerrainFaceDirection)i));
    }

}

PlanetaryMesh::~PlanetaryMesh(){}

void PlanetaryMesh::Update(Camera camera){
    for (int i = 0; i < faces.size(); i++) {
        faces.at(i).Update(camera);
    }
}

void PlanetaryMesh::Render(Camera camera){
    for (int i = 0; i < faces.size(); i++) {
        faces.at(i).Render(camera);
    }
}

int PlanetaryMesh::GetVertexNumber(){
    int vertexNumber = 0;
    for (int i = 0; i < faces.size(); i++) {
        vertexNumber += faces.at(i).GetVertexNumber();
    }
    
    return vertexNumber;
}
