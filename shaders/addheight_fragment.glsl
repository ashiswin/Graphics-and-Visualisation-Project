#version 400

in vec2 pass_texCoords;
in vec2 pass_position;

out vec4 frag_colour;

uniform float amount;
uniform vec2 location;
uniform sampler2D textureSampler;
uniform float width;
uniform float height;

void main() {
    float deltaX = 1 / width;
    float deltaY = 1 / height;

    vec4 texel;
    if (pass_position.x < -0.999) {

        float x = pass_texCoords.x;
        float y = -pass_texCoords.y;

        float curr_height = texture(textureSampler, vec2(x, y)).g;
        float averageHt = 0;

        averageHt += texture(textureSampler, vec2(x-deltaX, y)).g;
        averageHt += texture(textureSampler, vec2(x+deltaX, y)).g;
        averageHt += texture(textureSampler, vec2(x-deltaX, y+deltaY)).g;
        averageHt += texture(textureSampler, vec2(x-deltaX, y-deltaY)).g;
        averageHt += texture(textureSampler, vec2(x+deltaX, y-deltaY)).g;
        averageHt += texture(textureSampler, vec2(x+deltaX, y+deltaY)).g;
        averageHt += texture(textureSampler, vec2(x, y+deltaY)).g;
        averageHt += texture(textureSampler, vec2(x, y-deltaY)).g;

        // taking average
        averageHt = averageHt/8;
        // texel = texture(textureSampler, vec2(pass_texCoords.x, -pass_texCoords.y)) + vec4(amount, averageHt - curr_height, 0, 0);
        texel = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else {
        texel = vec4(0.0, 0.0, 1.0, 1.0);//texture(textureSampler, vec2(pass_texCoords.x, -pass_texCoords.y));
    }

    frag_colour = vec4(, 1.0, 1.0);
    // frag_colour = vec4(1.0, 1.0, 1.0, 1.0);
}
