//
//  ActiveShaders.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#include <stdio.h>

#include "ActiveShaders.h"

Shader* ActiveShaders::TerrainShader;
Shader* ActiveShaders::TreeModelShader;
Shader* ActiveShaders::GrassModelShader;

void ActiveShaders::Init(){
    TerrainShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMeshShader.vs",
                               "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TerrainMeshShader.fs");
    
    TreeModelShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TreeModelShader.vs",
                                 "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/TreeModelShader.fs");
    
    GrassModelShader = new Shader("/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/GrassModelShader.vs",
                                 "/Users/gabrielepadovani/Desktop/Code/C++/OpenGL/OpenGL/res/shaders/GrassModelShader.fs");

}
