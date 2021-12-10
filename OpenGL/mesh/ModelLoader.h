//
//  ModelLoader.h
//  OpenGL
//
//  Created by Gabriele Padovani on 06/12/21.
//

#ifndef ModelLoader_h
#define ModelLoader_h

#include <stdio.h>
#include <string>

#include "ModelMesh.h"

#include "../vendor/tiny_obj_loader/tiny_obj_loader.h"

class ModelLoader {
private:
    
public:
    static void Load(const std::string& path, ModelMesh& mesh);
};


#endif /* ModelLoader_h */
