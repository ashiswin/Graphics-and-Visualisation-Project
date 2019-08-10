#version 400

in vec3 position;
// in vec2 texCoords;
// in vec3 normals;

out vec2 pass_texCoords;
out float height;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform sampler2D heightTexture;

uniform vec4 plane;

void main() {
  vec4 worldPosition = modelMatrix * vec4(position, 1.0);
  gl_Position = projectionMatrix * viewMatrix * worldPosition;
//   gl_ClipDistance[0] = dot(worldPosition, plane);

  vec4 texel = texture(heightTexture, position.xz);
//   worldPosition = vec4(worldPosition.x, worldPosition.y, worldPosition.z + texel.r, worldPosition.w); 
  height = (texel.r + texel.b + texel.g)*256.0/3.0;
  pass_texCoords = position.xz;
}