#version 120

uniform sampler2DRect tex0;
uniform sampler2DRect imageMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(imageMask, texCoordVarying);
    
    float a = texel0.a;
    if(texel1.r == 0 && texel1.g == 0 && texel1.b == 0) {
        
        gl_FragColor = vec4(1,0,0,0);

    } else {
        gl_FragColor = vec4(texel0.rgb, a);

    }
        

}
