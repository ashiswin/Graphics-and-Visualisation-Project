#version 400

in vec4 pass_texel;
in vec2 pass_texCoords;
in vec4 clipSpace;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in vec3 position_out;

out vec4 frag_colour;

uniform mat4 modelMatrix;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 center;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main() {

  vec2 ndc = (clipSpace.xy/clipSpace.w) / 2.0 + 0.5;
  vec2 refractTexCoords = vec2(ndc.x, ndc.y);
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

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
  vec4 refractColor = texture(refractionTexture, refractTexCoords);
  
  float distance = sqrt(pow(position_out[0] - (+127.0), 2) + pow(position_out[1] - (0.0), 2) + pow(position_out[2] - (-180.0), 2));
  
  // if (distance < 60.0) {
    frag_colour = mix(reflectColor, refractColor, dot(normalize(toCameraVector), vec3(0, 1, 0)));
    if (reflectedDirection.y < 0.0)
      frag_colour = mix(frag_colour, vec4(1.0, 1.0, 1.25, 1.0), 0.1);
    // frag_colour = reflectColor;
  // }
  // else {
    // discard;
  // }
  // frag_colour = vec4(1, 0, 0, 1);
}