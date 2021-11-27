//
//  MeshHeight.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#include "MeshHeight.h"

FastNoiseLite MeshHeight::noise = FastNoiseLite();
std::vector<MeshHeightLevel> MeshHeight::heightLevels = std::vector<MeshHeightLevel>(QuadtreeSettings::MaxSubdivisions);

void MeshHeight::Init(){
    for (int i = 0; i < QuadtreeSettings::MaxSubdivisions; i++) {
        
        heightLevels[i].multiplier = (QuadtreeSettings::MaxSubdivisions-i) * 0.5;
        heightLevels[i].frequency = i / (float)QuadtreeSettings::MaxSubdivisions + 0.1;
        heightLevels[i].scale = 0.1f;
        
    }
}

float MeshHeight::GetApproximateHeight(float x, float y, float z){
    float height = noise.GetNoise(x * heightLevels[0].scale, y * heightLevels[0].scale, z * heightLevels[0].scale) * heightLevels[0].multiplier;
    if (height < 0) height = 0;
    return height;
}

float MeshHeight::GetHeight(float x, float y, float z, int LOD){
    float height = 0;
    
    for (int i = 0; i < LOD; i++) {
        
        noise = FastNoiseLite();
        noise.SetFrequency(heightLevels[i].frequency);
        
        float noiseLevelHeight = noise.GetNoise(x * heightLevels[i].scale,
                                                y * heightLevels[i].scale,
                                                z * heightLevels[i].scale) * heightLevels[i].multiplier;
        height += noiseLevelHeight;
    }
    
    
    if (height < 0) height = 0;
    return height;
}
