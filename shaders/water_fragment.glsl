#version 400

in vec4 pass_texel;

out vec4 frag_colour;

void main() {
  frag_colour = vec4(pass_texel.r, pass_texel.g, 1.0, 1.0);
}