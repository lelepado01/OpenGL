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
    static FastNoiseLite largeNoise;
    static FastNoiseLite smallNoise;

public:
    static float SmallFrequency;
    static float SmallScale;
    static float SmallMultiplier;
    
    static float LargeFrequency;
    static float LargeMultiplier;
    static float LargeScale;

public:
    static void Init(); 
    static float GetHeight(float x, float y, float z);
};

#endif /* MeshHeight_h */
