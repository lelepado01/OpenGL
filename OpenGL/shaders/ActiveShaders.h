//
//  ActiveShaders.hpp
//  OpenGL
//
//  Created by Gabriele Padovani on 20/11/21.
//

#ifndef ActiveShaders_h
#define ActiveShaders_h

#include <stdio.h>

#include "Shader.h"

class ActiveShaders {
public:
    static Shader* TerrainShader;
    
public:
    static void Init();
};

#endif /* ActiveShaders_hpp */
