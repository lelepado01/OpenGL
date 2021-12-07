//
//  ActiveShaders.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include <stdio.h>

#include "ActiveShaders.h"

Shader* ActiveShaders::TerrainShader;
Shader* ActiveShaders::ModelShader;

void ActiveShaders::Init(){
    TerrainShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMeshShader.vs",
                               "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMeshShader.fs");
    
    ModelShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/ModelShader.vs",
                             "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/ModelShader.fs");
}
