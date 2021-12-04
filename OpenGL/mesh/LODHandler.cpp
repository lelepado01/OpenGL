//
//  LODHandler.cpp
//  OpenGL
//
//  Created by Gabriele Padovani on 04/12/21.
//

#include "LODHandler.h"

std::vector<float> LODHandler::distances = std::vector<float>();

void LODHandler::Init(){
    for (int i = 0; i < QuadtreeSettings::MaxSubdivisions; i++) {
        distances.push_back((QuadtreeSettings::MaxSubdivisions - i) * 10);
    }
}

int LODHandler::GetLevelOfDetail(float distance){
    for (int i = ((int)distances.size())-1; i < 0; i--) {
        if (distance < distances.at(i)){
            return i;
        }
    }
    
    return QuadtreeSettings::MaxSubdivisions;
}
