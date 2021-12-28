//
//  PlanetSettings.h
//  OpenGL
//
//  Created by Gabriele Padovani on 16/12/21.
//

#ifndef PlanetSettings_h
#define PlanetSettings_h

#include "QuadtreeSettings.h"

class PlanetSettings {
public:
    constexpr static float PlanetRadius = QuadtreeSettings::InitialWidth; 
    constexpr static float SeaLevel = PlanetRadius + 1.4;

};


#endif /* PlanetSettings_h */
