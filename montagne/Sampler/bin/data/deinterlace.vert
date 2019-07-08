#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;
uniform float opacity;

in vec4 position;
in vec2 texcoord;
in vec4 normal;
in vec4 color;

out vec2 texCoordVarying;

void main() {
#ifdef INTEL_CARD
    color = vec4(1.0); // for intel HD cards
    normal = vec4(1.0); // for intel HD cards
#endif
    
    texCoordVarying = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
