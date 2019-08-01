#version 400

in vec4 pass_texel;

out vec4 frag_colour;

void main() {
  frag_colour = pass_texel;//vec4(pass_texel.xy, 1.0, 1.0);
}