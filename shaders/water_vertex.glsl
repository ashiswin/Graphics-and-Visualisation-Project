#version 400

in vec3 position;

out vec4 pass_texel;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform float width;
uniform float height;

out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform vec3 lightDirection;

uniform sampler2D textureSampler;

void main() {
    vec2 texCoords = vec2(position.x / width, -position.z / height);

    vec4 texel = texture(textureSampler, texCoords);
    pass_texel = texel;

    vec4 worldPosition = modelMatrix * vec4(position.x, position.y + texel.r, position.z, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    vec3 normal = vec3(texel.b, sqrt(1.0 - dot(texel.ba, texel.ba)), texel.a);

    surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
    toLightVector = -lightDirection;
    toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}