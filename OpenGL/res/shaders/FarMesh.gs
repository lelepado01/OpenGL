#version 410 core

in VS_OUT {
    vec3 objectNormal;
    vec3 fragPos;
    vec3 cameraPos;
    
    Material material;
    Light light;
    
    mat4 modelViewProjMatrix;
} gs_in[];

out vec3 objectNormal;
out vec3 fragPos;
out vec3 cameraPos;

out Material material;
out Light light;

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;


//float random (in vec2 st) {
//    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
//}
//
//// 2D Noise based on Morgan McGuire @morgan3d
//// https://www.shadertoy.com/view/4dS3Wd
//float noise (in vec2 st) {
//    vec2 i = floor(st);
//    vec2 f = fract(st);
//
//    // Four corners in 2D of a tile
//    float a = random(i);
//    float b = random(i + vec2(1.0, 0.0));
//    float c = random(i + vec2(0.0, 1.0));
//    float d = random(i + vec2(1.0, 1.0));
//
//    // Smooth Interpolation
//
//    // Cubic Hermine Curve.  Same as SmoothStep()
//    vec2 u = f*f*(3.0-2.0*f);
//    // u = smoothstep(0.,1.,f);
//
//    // Mix 4 coorners percentages
//    return mix(a, b, u.x) +
//            (c - a)* u.y * (1.0 - u.x) +
//            (d - b) * u.x * u.y;
//}

void main() {
    
    for(int i = 0; i < gl_in.length(); i++){
    
        gl_Position = gs_in[i].modelViewProjMatrix * gl_in[i].gl_Position;
    
        objectNormal = gs_in[i].objectNormal;
        fragPos = gs_in[i].fragPos;
        cameraPos = gs_in[i].cameraPos;
        material = gs_in[i].material;
        light = gs_in[i].light;
    
        EmitVertex();
    }
    EndPrimitive();
    
    //for(int i = gl_in.length() -100; i < gl_in.length() ; i++){
    //
    //    gl_Position = gs_in[i].modelViewProjMatrix * (gl_in[i].gl_Position + vec4(-100,100,0,0));
    //
    //    objectNormal = gs_in[i].objectNormal;
    //    fragPos = gs_in[i].fragPos;
    //    cameraPos = gs_in[i].cameraPos;
    //    material = gs_in[i].material;
    //    light = gs_in[i].light;
    //
    //    EmitVertex();
    //}
    //EndPrimitive();

}
