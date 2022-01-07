//
//  MeshHeight.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 12/11/21.
//

#include "MeshHeightHandler.h"

FastNoiseLite MeshHeightHandler::noise = FastNoiseLite();
std::vector<MeshHeightLevel> MeshHeightHandler::heightLevels = std::vector<MeshHeightLevel>(QuadtreeSettings::MaxSubdivisions);

void MeshHeightHandler::Init(){
    
    for (int i = 0; i < heightLevels.size(); i++) {
        heightLevels[i].enabled = false;
    }
    
    heightLevels[0].enabled = true;
    heightLevels[0].multiplier = 3.5f;
    heightLevels[0].frequency = 0.08f;
    heightLevels[0].scale = 0.008f;
    
    heightLevels[1].enabled = true;
    heightLevels[1].multiplier = 1.5f;
    heightLevels[1].frequency = 0.7f;
    heightLevels[1].scale = 0.009f;
    
    heightLevels[2].enabled = true;
    heightLevels[2].multiplier = 1.5f;
    heightLevels[2].frequency = 0.6f;
    heightLevels[2].scale = 0.1f;

    heightLevels[3].enabled = true;
    heightLevels[3].multiplier = 0.7f;
    heightLevels[3].frequency = 0.7f;
    heightLevels[3].scale = 0.5f;
}

float MeshHeightHandler::GetHeight(float x, float y, float z, int LOD){
    float seaLevel = 1.2f;
    
    float height = 0;
    
    for (int i = 0; i < heightLevels.size(); i++) {
        if (heightLevels[i].enabled){
            noise.SetFrequency(heightLevels[i].frequency);
            
            float noiseLevelHeight = noise.GetNoise(x * heightLevels[i].scale,
                                                    y * heightLevels[i].scale,
                                                    z * heightLevels[i].scale) * heightLevels[i].multiplier;
            
            if (height < seaLevel && i > 2){
                continue;
            }
            
            height += noiseLevelHeight * noiseLevelHeight;
        }
    }
    
    if (height < seaLevel) height = seaLevel;
    return height;
}
