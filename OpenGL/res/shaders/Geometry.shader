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
layout (points, max_vertices = 1) out;

void main() {
    
    //gl_Position = gl_in[0].gl_Position;
    //EmitVertex();
    //EndPrimitive();
    
    objectNormal = gs_in[0].objectNormal;
    fragPos = gs_in[0].fragPos;
    cameraPos = gs_in[0].cameraPos;
    material = gs_in[0].material;
    light = gs_in[0].light; 
}
