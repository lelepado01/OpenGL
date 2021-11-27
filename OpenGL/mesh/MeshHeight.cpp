//
//  MeshHeight.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#include "MeshHeight.h"

FastNoiseLite MeshHeight::noise = FastNoiseLite();
std::vector<float> MeshHeight::levelFrequency = std::vector<float>(QuadtreeSettings::MaxSubdivisions);
std::vector<float> MeshHeight::levelMultiplier = std::vector<float>(QuadtreeSettings::MaxSubdivisions);
std::vector<float> MeshHeight::levelScale = std::vector<float>(QuadtreeSettings::MaxSubdivisions);

void MeshHeight::Init(){
    for (int i = 0; i < QuadtreeSettings::MaxSubdivisions; i++) {
        
        levelMultiplier[i] = (QuadtreeSettings::MaxSubdivisions-i) * 0.5;
        levelFrequency[i] = (i+1) / ((float)QuadtreeSettings::MaxSubdivisions+1);
        levelScale[i] = 0.1f;
        
    }
}

float MeshHeight::GetApproximateHeight(float x, float y, float z){
    float height = noise.GetNoise(x * levelScale[0], y * levelScale[0], z * levelScale[0]) * levelMultiplier[0];
    if (height < 0) height = 0;
    return height;
}

float MeshHeight::GetHeight(float x, float y, float z, int LOD){
    float height = 0;
    
    for (int i = 0; i < LOD; i++) {
        
        noise = FastNoiseLite();
        noise.SetFrequency(levelFrequency[i]);
        
        float noiseLevelHeight = noise.GetNoise(x * levelScale[i], y * levelScale[i], z * levelScale[i]) * levelMultiplier[i];
        height += noiseLevelHeight;
    }
    
    
    if (height < 0) height = 0;
    return height;
}
