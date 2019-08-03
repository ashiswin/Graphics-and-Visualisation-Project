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

    float x = gl_FragCoord.x / width;
    float y = gl_FragCoord.y / height;

    vec2 coord = vec2(x, y);
    
    vec4 texel = texture(textureSampler, coord);

    float drop = max(0.0, 1.0 - length(location - gl_FragCoord.xy) / 2);
    drop = 0.5 - cos(drop * PI) * 0.5;
    texel.r += drop * amount;

    frag_colour = texel;
}
