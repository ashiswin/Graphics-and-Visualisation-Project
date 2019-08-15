#version 400

in vec3 regularPosition;
in vec3 newPosition;
in vec3 rayDirection;
in vec2 pass_texCoords;
in vec2 pass_position;

out vec4 frag_color;

const float AIR = 1.0;
const float WATER = 1.33;

uniform vec3 lightDirection;

void main() {
    float oldArea = length(dFdx(regularPosition)) * length(dFdy(regularPosition));
    float newArea = length(dFdx(newPosition)) * length(dFdy(newPosition));
    frag_color = vec4(oldArea / newArea * 0.2, 1, 0.0, 0.0);

    vec3 refractedLight = refract(-lightDirection, vec3(0.0, 1.0, 0.0), AIR / WATER);\

    vec3 dir = (vec3(0, 0, 0) - newPosition) / 0.1;\
    vec3 area = cross(dir, refractedLight);\
    float shadow = dot(area, area);\

    float dist = dot(dir, -refractedLight);\
    shadow = 1.0 + (shadow - 1.0) / (0.05 + dist * 0.025);\
    shadow = clamp(1.0 / (1.0 + exp(-shadow)), 0.0, 1.0);\
    shadow = mix(1.0, shadow, clamp(dist * 2.0, 0.0, 1.0));\
    frag_color.g = shadow * 2;


    // frag_color = vec4(pass_texCoords.x, pass_texCoords.y, 0, 1);
    // frag_color = vec4(pass_position.x, pass_position.y, 0, 1);
    // frag_color = vec4(1, 1, 1, 1);
    // frag_color = gl_FragCoord;
}