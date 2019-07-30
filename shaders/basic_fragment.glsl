#version 400

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 frag_colour;

uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

void main() {
  vec3 normal = normalize(surfaceNormal);
  vec3 lightVector = normalize(toLightVector);
  
  float brightness = max(dot(normal, lightVector), 0.0);

  vec3 diffuse = brightness * lightColor;

  vec3 cameraVector = normalize(toCameraVector);
  vec3 lightDirection = -lightVector;
  vec3 reflectedDirection = reflect(lightDirection, normal);

  float specular = max(dot(reflectedDirection, cameraVector), 0.0);
  float dampedFactor = pow(specular, shineDamper);
  vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

  frag_colour = vec4(diffuse, 1.0) + vec4(finalSpecular, 1.0);
  // frag_colour = vec4(1.0, 1.0, 1.0, 1.0);
}