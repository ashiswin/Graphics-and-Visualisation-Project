#version 400

in vec4 pass_texel;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 frag_colour;

uniform mat4 modelMatrix;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

void main() {
  vec3 ambient = 0.1 * lightColor;

  vec3 normal = normalize(surfaceNormal);

  vec3 lightVector = normalize(toLightVector);
  
  float brightness = max(dot(normal, lightVector), 0.0);

  vec3 diffuse = brightness * lightColor;

  frag_colour = vec4(ambient, 1.0) + vec4(diffuse, 1.0);//vec4(pass_texel.xy, 1.0, 1.0);
}