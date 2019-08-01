#version 400

in vec2 pass_texCoords;

out vec4 frag_colour;

void main() {
  frag_colour = vec4(1.0, 1.0, 1.0, 1.0);
  // frag_colour = vec4(normal, 1.0);
//   frag_colour = vec4(0.0, pass_texCoords.x,, 1.0);
}