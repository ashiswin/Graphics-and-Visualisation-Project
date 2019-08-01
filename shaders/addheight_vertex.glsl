#version 400

in vec3 position;
in vec2 texCoords;

out vec2 pass_texCoords;
out vec2 pass_position;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float width;
uniform float height;

void main() {
    gl_Position = vec4(position, 1.0);

    // pass_texCoords.x = position.x / width;
    // pass_texCoords.y = position.z / height;
    pass_texCoords = texCoords;
    pass_position = position.xz;
    
}