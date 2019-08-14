#version 400

out vec4 frag_colour;

const float PI = 3.141592653589793;

uniform float amount;
uniform vec2 location;
uniform sampler2D textureSampler;
uniform float width;
uniform float height;

void main() {
    float deltaX = 1 / width;
    float deltaY = 1 / height;

    vec2 frag = gl_FragCoord.xy + vec2(0.5, 0.5);

    float x = frag.x / width;
    float y = frag.y / height;

    vec2 coord = vec2(x, y);
    
    vec4 texel = texture(textureSampler, coord);

    float drop = max(0.0, 1.0 - length(location - frag) / 20);
    drop = 0.5 - cos(drop * PI) * 0.5;
    texel.r += drop * amount;

    frag_colour = texel;
}
