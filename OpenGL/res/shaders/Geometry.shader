#version 410 core

in VS_OUT {
    vec3 objectNormal;
    vec3 fragPos;
    vec3 cameraPos;
    Material material;
    Light light;
} gs_in[];

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;

out Material material;
out Light light;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main() {
    
    for(int i = 0; i < gl_in.length(); i++){
        
        gl_Position = gl_in[i].gl_Position;
        
        objectNormal = gs_in[i].objectNormal;
        fragPos = gs_in[i].fragPos;
        cameraPos = gs_in[i].cameraPos;
        material = gs_in[i].material;
        light = gs_in[i].light;
        
        EmitVertex();
    }
    EndPrimitive();
}
