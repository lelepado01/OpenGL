//
//  ActiveShaders.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include <stdio.h>

#include "ActiveShaders.h"

Shader* ActiveShaders::TerrainShader;

void ActiveShaders::Init(){
    TerrainShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMesh.vs", 
                               "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMesh.fs");
}
