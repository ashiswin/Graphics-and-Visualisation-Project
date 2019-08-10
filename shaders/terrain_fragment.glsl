#version 400

in vec2 pass_texCoords;
in float height;

out vec4 frag_colour;

void main() {
  
  if (height > 57) {
    frag_colour = vec4(0.0, 1.0, 0.0, 1.0);
  }
  else {
    frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
  }
}