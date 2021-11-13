//
//  MeshHeight.h
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#ifndef MeshHeight_h
#define MeshHeight_h

#include <stdio.h>

#include "../vendor/fast_noise/FastNoiseLite.h"

class MeshHeight {
private:
    FastNoiseLite largeNoise;
    FastNoiseLite smallNoise;

public:
    float SmallFrequency = 0.4f;
    float SmallScale = 0.2f;
    float SmallMultiplier = 1.5f;
    
    float LargeFrequency = 0.1f;
    float LargeMultiplier = 3.0f;
    float LargeScale = 0.2f;

public:
    MeshHeight();
    ~MeshHeight();
    
    float GetHeight(float x, float y); 
};

#endif /* MeshHeight_h */
