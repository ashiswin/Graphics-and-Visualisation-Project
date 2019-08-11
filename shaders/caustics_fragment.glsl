#version 400

in vec3 regularPosition;
in vec3 newPosition;
in vec3 rayDirection;

out vec4 frag_color;

void main() {
    float oldArea = length(dFdx(regularPosition)) * length(dFdy(regularPosition));\
    float newArea = length(dFdx(newPosition)) * length(dFdy(newPosition));\
    // frag_color = vec4(oldArea / newArea * 0.2, 1.0, 0.0, 0.0);
    frag_color = vec4(1, 1, 1, 1);
}