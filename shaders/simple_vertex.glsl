#version 400

in vec3 position;
in vec2 texCoords;
in vec3 normal;

out vec2 pass_texCoords;
out vec3 out_position;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec4 plane;

void main() {
  vec4 worldPosition = modelMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
  gl_ClipDistance[0] = dot(worldPosition, plane);
  
  pass_texCoords = position.xz;
  out_position = position;
}