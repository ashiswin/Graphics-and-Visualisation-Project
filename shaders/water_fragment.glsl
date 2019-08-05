#version 400

in vec4 pass_texel;
in vec2 pass_texCoords;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 frag_colour;

uniform mat4 modelMatrix;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main() {
  vec3 ambient = 0.1 * lightColor;

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

  vec4 reflectColor = texture(reflectionTexture, pass_texCoords);
  vec4 refractColor = texture(refractionTexture, pass_texCoords);
  
  frag_colour = mix(reflectColor, refractColor, dot(normalize(toCameraVector), vec3(0, 1, 0)));

  // frag_colour = vec4(ambient, 1.0) + vec4(diffuse, 1.0);//vec4(pass_texel.xy, 1.0, 1.0);
}