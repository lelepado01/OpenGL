//
//  TreeModelMesh.h
//  OpenGL
//
//  Created by Gabriele Padovani on 28/12/21.
//

#ifndef TreeModelMesh_h
#define TreeModelMesh_h

#include <stdio.h>

#include "ModelMesh.h"

class TreeModelMesh : public ModelMesh {
protected:
    void copyData(const TreeModelMesh& mesh);

public:
    TreeModelMesh();
    ~TreeModelMesh();
    TreeModelMesh(const TreeModelMesh& mesh);
    TreeModelMesh(TreeModelMesh&& mesh);
    TreeModelMesh& operator=(const TreeModelMesh& mesh);
};

#endif /* TreeModelMesh_h */
