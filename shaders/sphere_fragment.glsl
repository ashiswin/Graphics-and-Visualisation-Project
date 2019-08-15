#version 400

in vec3 surfaceNormal;
in vec3 toLightVector;
// in vec3 toCameraVector;
in vec2 pass_texCoords;
in vec3 out_position;

out vec4 frag_colour;

uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform vec3 lightDirection;

const float AIR = 1.0;
const float WATER = 1.33;

  
vec3 getSphereColor(vec3 point, vec3 sphereNormal) {
    vec3 color = vec3(0.5);

    /* caustics */
    // vec3 sphereNormal = (point) / 0.1;
    vec3 refractedLight = refract(toLightVector, vec3(0.0, 1.0, 0.0), AIR / WATER);
    float diffuse = max(0.0, dot(-refractedLight, sphereNormal)) * 0.5;
    vec3 norm = (point + vec3(1, 1, 1)) / 2;
    norm = norm / 2 + vec3(0.5, 0.5, 0.5);
    vec4 caustic = texture(textureSampler, vec2(norm.x, norm.z));
    diffuse *= caustic.r * 4.0;
    color += diffuse;
    // if (norm.z < 0.75) color = vec3(1, 0, 0);
    return color;
}

void main() {
  vec3 normal = normalize(surfaceNormal);

  vec3 lightVector = normalize(toLightVector);
  
  float brightness = max(dot(normal, lightVector), 0.0);

  vec3 diffuse = brightness * vec3(1.0, 1.0, 1.0);
//   frag_colour = vec4(out_position, 1.0);
  frag_colour = vec4(getSphereColor(out_position, normal), 1.0);
//   vec4 info = texture(textureSampler, out_position.xz * 0.5 + 0.5);
//   if (out_position.y < info.r) {
    frag_colour.rgb *= vec3(0.4, 0.9, 1.0) * 1.2;
    // frag_colour = vec4(normal, 1.0);
//   }
//   frag_colour = vec4(1.0, 1.0, 1.0, 1.0);
}