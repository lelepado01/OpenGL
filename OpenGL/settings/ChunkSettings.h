//
//  ChunkSettings.h
//  OpenGL
//
//  Created by Gabriele Padovani on 17/11/21.
//

#ifndef ChunkSettings_h
#define ChunkSettings_h


class ChunkSettings {
public:
    static const int VerticesPerSide = 10;
    static const int DistanceBetweenVertices = 10;
    static const int ChunkSize = VerticesPerSide * DistanceBetweenVertices;
};

#endif /* ChunkSettings_h */