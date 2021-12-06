//
//  MeshHeight.h
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#ifndef MeshHeight_h
#define MeshHeight_h

#include <stdio.h>
#include <iostream>
#include <vector>

#include "../vendor/fast_noise/FastNoiseLite.h"
#include "../settings/QuadtreeSettings.h"

struct MeshHeightLevel {
    bool enabled; 
    float frequency;
    float multiplier;
    float scale;
};

class MeshHeightHandler {
private:
    static FastNoiseLite noise;
    static std::vector<MeshHeightLevel> heightLevels;

public:
    static void Init();
    static float GetHeight(float x, float y, float z, int LOD);
};

#endif /* MeshHeight_h */
