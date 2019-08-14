#version 400

out vec4 frag_colour;

uniform sampler2D textureSampler;

uniform float width;
uniform float height;

void main() {
    float deltaX = 1 / width;
    float deltaY = 1 / height;

    float x = gl_FragCoord.x / width;
    float y = gl_FragCoord.y / height;
    
    vec4 texel = texture(textureSampler, vec2(x, y));

    // Get new velocity
    float averageHt = 0;

    averageHt += texture(textureSampler, vec2(x-deltaX, y)).r;
    averageHt += texture(textureSampler, vec2(x+deltaX, y)).r;
    averageHt += texture(textureSampler, vec2(x-deltaX, y+deltaY)).r;
    averageHt += texture(textureSampler, vec2(x-deltaX, y-deltaY)).r;
    averageHt += texture(textureSampler, vec2(x+deltaX, y-deltaY)).r;
    averageHt += texture(textureSampler, vec2(x+deltaX, y+deltaY)).r;
    averageHt += texture(textureSampler, vec2(x, y+deltaY)).r;
    averageHt += texture(textureSampler, vec2(x, y-deltaY)).r;

    averageHt = averageHt * 0.125;

    texel.g += (averageHt - texel.r) * 2;
    texel.g *= 0.99;

    // Step simulation forward
    texel.r += texel.g;

    frag_colour = texel;
}
