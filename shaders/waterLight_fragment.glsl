#version 400 core

in vec2 pass_texCoords;

out vec4 frag_color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(void) {
    vec4 reflectColor = texture(reflectionTexture, pass_texCoords);
    vec4 refractColor = texture(refractionTexture, pass_texCoords);

    frag_color = mix(reflectColor, refractColor, 0);
}