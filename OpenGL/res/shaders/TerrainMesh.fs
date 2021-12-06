#version 410 core

in vec3 objectNormal;
in vec3 fragPos;
in vec3 cameraPos;

in Material material;
in Light light;

out vec4 fragColor;





//ATM parameters
const highp float earthRadius = 4096;

const highp float stratoHeight = 1000;

//Height at which the effect is replaced by a blue background
const highp float minHeigth = 1500;
const highp float maxHeight = 4300;

//Multicolor gradient
highp vec4 whiteSky = vec4(1.0, 1.0, 1.0, 1.0);
highp vec4 blueSky = vec4(32.0 / 256.0, 173.0 / 256.0, 249.0 / 256.0, 1.0);
highp vec4 darkSpace = vec4(1.0, 0.0, 0.0, 0.0);
highp vec4 groundSkyColor = mix(blueSky, whiteSky, smoothstep(0.0, 1.0, 0.5));


bool intersectionsWithAtmosphere(highp vec3 o, highp vec3 d,
                                 out highp vec3 p1,
                                 out highp vec3 p2){
  //http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
  
  highp float a = dot(d,d);
  highp float b = 2.0 * dot(o,d);
  highp float r = earthRadius + maxHeight - distance(cameraPos, vec3(0,0,0)); //Earth radius
  highp float c = dot(o,o) - (r*r);
  
  highp float q1 = (b*b) - 4.0 * a * c;
  
  r = earthRadius + stratoHeight; //Atm. radius
  c = dot(o,o) - (r*r);
  
  highp float q2 = (b*b) - 4.0 * a * c;
  bool valid = (q1 < 0.0) && (q2 > 0.0);
  
  if (valid){
    highp float sq = sqrt(q2);
    highp float t1 = (-b - sq) / (2.0*a);
    highp float t2 = (-b + sq) / (2.0*a);
    
    if (t1 < 0.0 && t2 < 0.0){
      return false;
    }
    
    p1 = o + d * max(min(t1,t2), 0.0);
    p2 = o + d * max(t1,t2);
  }
  return valid;
}

highp float getRayFactor(highp vec3 o, highp vec3 d){
  
  //Ray density calculations explained in: https://github.com/amazingsmash/AtmosphericShaders
  
  //Scaling the scene down to improve floating point calculations
  d /= 1000.0;
  o /= 1000.0;
  highp float er = earthRadius / 1000.0;
  highp float sh = (stratoHeight + earthRadius) / 1000.0;
  
  highp float ld = dot(d,d);
  highp float pdo = dot(d,o);
  
  highp float dx = d.x;
  highp float dy = d.y;
  highp float dz = d.z;
  
  highp float ox = o.x;
  highp float oy = o.y;
  highp float oz = o.z;

  highp float dox2 = (dx + ox) * (dx + ox);
  highp float doy2 = (dy + oy) * (dy + oy);
  highp float doz2 = (dz + oz) * (dz + oz);
  
  highp float ox2 = ox * ox;
  highp float oy2 = oy * oy;
  highp float oz2 = oz * oz;
  
  highp float dx2 = dx * dx;
  highp float dy2 = dy * dy;
  highp float dz2 = dz * dz;
  
  highp float s = (((dx*(dx + ox) + dy*(dy + oy) + dz*(dz + oz))*
    sqrt(dox2 + doy2 + doz2))/ld -
   (sqrt(ox2 + oy2 + oz2)*pdo)/ld - 2.*sh +
   ((dz2*(ox2 + oy2) - 2.0*dx*dz*ox*oz - 2.0*dy*oy*(dx*ox + dz*oz) +
     dy2*(ox2 + oz2) + dx2*(oy2 + oz2))*
    log(dx*(dx + ox) + dy*(dy + oy) + dz*(dz + oz) +
        sqrt(ld)*sqrt(dox2 + doy2 + doz2)))/pow(ld,1.5) -
   ((dz2*(ox2 + oy2) - 2.0*dx*dz*ox*oz - 2.0*dy*oy*(dx*ox + dz*oz) +
     dy2*(ox2 + oz2) + dx2*(oy2 + oz2))*
    log(sqrt(ld)*sqrt(ox2 + oy2 + oz2) + pdo))/pow(ld,1.5))/
  (2.*(er - 1.*sh));
  
  return s;
}








vec3 getColorFromHeight(float height){
    if (height < 4097.3) {
        return vec3(0.3,0.3,1);
    } else if (height < 4097.4 ) {
        return vec3(0.6,0.6,1);
    } else if (height < 4097.7 ) {
        return vec3(1,1,0);
    } else if (height < 4098 ) {
        return vec3(0,1,0);
    }  else if (height < 4099 ) {
        return vec3(0.1,0.7,0.1);
    }  else if (height < 4102 ) {
        return vec3(0.5,0.5,0.5);
    }
        
    return vec3(1,1,1);
}


vec3 getFaceNormal(vec3 position) {
    vec3 dx = vec3(dFdx(position.x), dFdx(position.y), dFdx(position.z));
    vec3 dy = vec3(dFdy(position.x), dFdy(position.y), dFdy(position.z));
    return normalize(cross(dy, dx));
}

void main() {

    vec3 norm = mix(getFaceNormal(fragPos), normalize(-objectNormal), 0.5f);

    vec3 lightDir = normalize(-light.direction);

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 specular = light.specular * (spec * material.specular);
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 result = (ambient + diffuse + specular) * getColorFromHeight(distance(fragPos, vec3(0,0,0)));
    fragColor = vec4(result, 1.0);
    
    
//    highp vec3 sp1, sp2;
//    bool valid = intersectionsWithAtmosphere(cameraPos, fragPos - cameraPos, sp1, sp2);
//    if (!valid){
//        //fragColor = darkSpace;
//    } else{
//        //Calculating color
//        highp float f = getRayFactor(sp1, sp2 - sp1) * 1.3;
//        
//        highp vec4 color = mix(darkSpace, blueSky, smoothstep(0.0, 1.0, f));
//        color = mix(color, whiteSky, smoothstep(0.7, 1.0, f));
//        
//        fragColor = mix(fragColor, color, 0.2);
//
//        //Calculating camera Height (for precision problems)
//        //Below a certain threshold float precision is not enough for calculations
//        highp float camHeight = length(cameraPos) - earthRadius;
//        fragColor = mix(fragColor, groundSkyColor, smoothstep(minHeigth, minHeigth / 4.0, camHeight));
//    }
}
