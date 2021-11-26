//
//  MeshHeight.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#include "MeshHeight.h"

float MeshHeight::SmallFrequency = 0.4f;
float MeshHeight::SmallScale = 0.2f;
float MeshHeight::SmallMultiplier = 1.5f;

float MeshHeight::LargeFrequency = 0.04f;
float MeshHeight::LargeMultiplier = 2.5f;
float MeshHeight::LargeScale = 0.2f;

FastNoiseLite MeshHeight::smallNoise = FastNoiseLite();
FastNoiseLite MeshHeight::largeNoise = FastNoiseLite();

void MeshHeight::Init(){
    smallNoise.SetFrequency(SmallFrequency);
    largeNoise.SetFrequency(LargeFrequency);
}

float MeshHeight::GetHeight(float x, float y, float z){
    smallNoise.SetFrequency(SmallFrequency);
    largeNoise.SetFrequency(LargeFrequency);
        
    float large = largeNoise.GetNoise(x * LargeScale, y * LargeScale, z * LargeScale) * LargeMultiplier;
    float small = smallNoise.GetNoise(x * SmallScale, y * SmallScale, z * SmallScale) * SmallMultiplier;
    
    float height = large * large * large + small;
    
    if (height < 0) height = 0;
    
    return height;
}
