#version 400

const float AIR = 1.0;
const float WATER = 1.33;

in vec3 position;

out vec3 regularPosition;
out vec3 newPosition;
out vec3 rayDirection;
out vec2 pass_texCoords;
out vec2 pass_position;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float width;
uniform float height;

uniform sampler2D textureSampler;

vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) {\
    vec3 tMin = (cubeMin - origin) / ray;\
    vec3 tMax = (cubeMax - origin) / ray;\
    vec3 t1 = min(tMin, tMax);\
    vec3 t2 = max(tMin, tMax);\
    float tNear = max(max(t1.x, t1.y), t1.z);\
    float tFar = min(min(t2.x, t2.y), t2.z);\
    return vec2(tNear, tFar);\
}

vec3 project(vec3 origin, vec3 ray, vec3 refractedLight) {
    vec2 tcube = intersectCube(origin, ray, vec3(-1.0, -1.0, -1.0), vec3(1.0, 2.0, 1.0));
    origin += ray * tcube.y;
    float tplane = (-origin.y - 1.0) / refractedLight.y;
    return origin + refractedLight * tplane;
}

void main() {
    vec2 texcoords = vec2((position.x + 1) / 2, (position.y + 1) / 2);
    pass_texCoords = texcoords;

    vec4 texel = texture(textureSampler, texcoords);

    vec3 normal = normalize(vec3(texel.b, sqrt(1.0 - dot(texel.ba, texel.ba)), texel.a));
    
    vec3 plainRefraction = refract(lightDirection, vec3(0.0, 1.0, 0.0), AIR / WATER);
    vec3 warpedRefraction = refract(lightDirection, normal, AIR / WATER);

    regularPosition = project(position.xzy, plainRefraction, plainRefraction);
    newPosition = project(position.xzy + vec3(0.0, texel.r, 0.0), warpedRefraction, plainRefraction);
    rayDirection = warpedRefraction;

    // gl_Position = vec4((newPosition.xz + plainRefraction.xz / plainRefraction.y), 0.0, 1.0);
    gl_Position = vec4((position + vec3(1, 1, 0)) * 0.5, 1);
    pass_position = gl_Position.xy;
    // pass_position = 0.75 * (newPosition.xz + plainRefraction.xz / plainRefraction.y);
}