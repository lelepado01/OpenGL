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

class MeshHeight {
private:
    static FastNoiseLite noise; 
    static std::vector<float> levelFrequency; 
    static std::vector<float> levelMultiplier;
    static std::vector<float> levelScale; 

public:
    static void Init();
    static float GetApproximateHeight(float x, float y, float z);
    static float GetHeight(float x, float y, float z, int LOD);
};

#endif /* MeshHeight_h */
