#version 400

in vec2 pass_texCoords;
in vec3 point;

out vec4 frag_colour;

uniform sampler2D textureSampler;
uniform sampler2D causticsTex;
uniform vec3 lightDirection;

const float IOR_AIR = 1.0;
const float IOR_WATER = 1.33;

void main() {
    vec4 texel = texture(textureSampler, vec2(pass_texCoords.x, -pass_texCoords.y));
    float scale = 0.5;
    vec3 refractedLight = -refract(-lightDirection, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    float diffuse = max(0.0, dot(refractedLight, vec3(0, 1, 0)));
    
    vec4 caustic = texture(causticsTex, 0.75 * (point.xz - point.y * refractedLight.xz / refractedLight.y) * 0.5 + 0.5);
    scale += diffuse * caustic.r * 2.0 * caustic.g;
    
    frag_colour = texel * scale;
    // frag_colour = caustic;
    // frag_colour = vec4(pass_texCoords.x, -pass_texCoords.y, 0, 1);
}