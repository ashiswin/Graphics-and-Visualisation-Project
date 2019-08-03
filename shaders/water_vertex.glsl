#version 400

in vec3 position;

out vec4 pass_texel;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float width;
uniform float height;

uniform sampler2D textureSampler;

void main() {
    vec2 texCoords = vec2(position.x / width, position.z / height);
    texCoords.y = -texCoords.y;

    vec4 texel = texture(textureSampler, texCoords);
    pass_texel = texel;

    vec4 worldPosition = modelMatrix * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}