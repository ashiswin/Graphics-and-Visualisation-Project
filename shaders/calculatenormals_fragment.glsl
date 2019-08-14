#version 400

out vec4 frag_colour;

uniform sampler2D textureSampler;

uniform float width;
uniform float height;

void main() {
    float deltaX = 1 / width;
    float deltaY = 1 / height;

    vec2 coord = vec2(gl_FragCoord.x / width, gl_FragCoord.y / height);
    
    vec4 texel = texture(textureSampler, coord);

    // Get new velocity
    vec3 dx = vec3(deltaX, texture(textureSampler, vec2(coord.x + deltaX, coord.y)).r - texel.r, 0.0);
    vec3 dy = vec3(0.0, texture(textureSampler, vec2(coord.x, coord.y + deltaY)).r - texel.r, deltaY);
    texel.ba = normalize(cross(dy, dx)).xz;

    frag_colour = texel;
}
