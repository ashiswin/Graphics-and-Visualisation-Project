#version 400

out vec4 frag_colour;

uniform sampler2D textureSampler;

uniform float width;
uniform float height;

void main() {
    float deltaX = 1 / width;
    float deltaY = 1 / height;

    vec2 frag = gl_FragCoord.xy + vec2(0.5, 0.5);

    float x = frag.x / width;
    float y = frag.y / height;
    
    vec4 texel = texture(textureSampler, vec2(x, y));

    // Get new velocity
    float averageHt = 0;

    // averageHt += texture(textureSampler, vec2(x-deltaX, y)).r;
    // averageHt += texture(textureSampler, vec2(x+deltaX, y)).r;
    averageHt += texture(textureSampler, vec2(x-deltaX, y+deltaY)).r;
    averageHt += texture(textureSampler, vec2(x-deltaX, y-deltaY)).r;
    averageHt += texture(textureSampler, vec2(x+deltaX, y-deltaY)).r;
    averageHt += texture(textureSampler, vec2(x+deltaX, y+deltaY)).r;
    // averageHt += texture(textureSampler, vec2(x, y+deltaY)).r;
    // averageHt += texture(textureSampler, vec2(x, y-deltaY)).r;

    averageHt = averageHt * 0.25;

    texel.g += (averageHt - texel.r) * 2;
    texel.g *= 0.995;

    // Step simulation forward
    texel.r += texel.g;

    frag_colour = texel;
}
