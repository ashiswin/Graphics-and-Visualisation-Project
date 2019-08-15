#version 400

in vec3 position;
in vec2 texCoords;
in vec3 normal;

out vec3 surfaceNormal;
out vec3 toLightVector;
// out vec3 toCameraVector;
out vec2 pass_texCoords;
out vec3 out_position;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main() {
  vec4 worldPosition = modelMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * viewMatrix * worldPosition;

  surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
  toLightVector = -lightDirection;
//   toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

  pass_texCoords = texCoords;
  out_position = position;
}