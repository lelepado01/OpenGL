//
//  MeshHeight.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#include "MeshHeight.h"

MeshHeight::MeshHeight(){
    smallNoise = FastNoiseLite();
    smallNoise.SetFrequency(SmallFrequency);
    
    largeNoise = FastNoiseLite();
    largeNoise.SetFrequency(LargeFrequency);
}

MeshHeight::~MeshHeight(){
    
}

float MeshHeight::GetHeight(float x, float y){
    smallNoise.SetFrequency(SmallFrequency);
    largeNoise.SetFrequency(LargeFrequency);
    
    float large = largeNoise.GetNoise(x * LargeScale, y * LargeScale) * LargeMultiplier;
    float small = smallNoise.GetNoise(x * SmallScale, y * SmallScale) * SmallMultiplier;
    
    float height = large * large * large + small;
    
    if (height < 0) height = 0;
    
    return height;
}
