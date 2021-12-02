//
//  QuadtreeSettings.h
//  OpenGL
//
//  Created by Gabriele Padovani on 21/11/21.
//

#ifndef QuadtreeSettings_h
#define QuadtreeSettings_h

class QuadtreeSettings {
public:
    static const int MaxSubdivisions = 10;
    static const int VerticesPerPatchSide = 50; // 100 ideally
    static const int InitialWidth = 4096;
    static const int SphereRadius = 1;

    constexpr static float MinDistanceToTerrain = 1.5;
};

#endif /* QuadtreeSettings_h */
