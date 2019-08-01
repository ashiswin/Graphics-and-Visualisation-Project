#version 400

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec2 pass_texCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
  vec4 worldPosition = modelMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * viewMatrix * worldPosition;

  pass_texCoords = texCoords;
}