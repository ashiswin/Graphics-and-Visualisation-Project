#version 400

in vec3 position;
in vec2 texCoords;

uniform float width;
uniform float height;

void main() {
    gl_Position = vec4(position, 1.0);
}