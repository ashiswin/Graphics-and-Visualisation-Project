#version 400

in vec2 pass_texCoords;

out vec4 frag_colour;

uniform sampler2D textureSampler;

void main() {
    vec4 texel = texture(textureSampler, vec2(pass_texCoords.x, -pas_texCoords.y));
s
    frag_colour = texel;
    // frag_colour = vec4(1, 1, 1, 1);
    // frag_colour = vec4(, 0, 0, 1);
}