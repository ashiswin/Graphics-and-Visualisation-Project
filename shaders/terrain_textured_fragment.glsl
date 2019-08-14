#version 400

in vec2 pass_texCoords;
in vec3 out_position;

out vec4 frag_colour;

uniform sampler2D textureSampler;
uniform sampler2D causticsTexture;
uniform vec3 lightDirection;

const float IOR_AIR = 1.0;
const float IOR_WATER = 1.33;

void main() {
    float scale = 0.5;\

    vec4 texel = texture(textureSampler, vec2(pass_texCoords.x, -pass_texCoords.y));
    vec3 refractedLight = -refract(-lightDirection, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);\
    vec3 normal = vec3(0.0, 1.0, 0.0);
    float diffuse = max(0.0, dot(refractedLight, normal));\
    vec2 positionCaustics = (out_position.xz - out_position.y * refractedLight.xz / refractedLight.y);
    positionCaustics.y = - positionCaustics.y;
    vec4 caustic = texture(causticsTexture, 0.75 * positionCaustics * 0.5 + 0.5);\
    scale += diffuse * caustic.r * 2.0 * caustic.g;\

    frag_colour = texel * scale;
}