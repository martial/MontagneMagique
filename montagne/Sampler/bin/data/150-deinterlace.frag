#version 150

uniform sampler2DRect tex0;
uniform float opacity;

in vec2 texCoordVarying;
out vec4 outputColor;


void main (void) 
{ 
	// we have to average like so:	
	// scanline 0 and 1 are (0 + 1) / 2.
	// scanline 2 and 3 are (2 + 3) / 2.
	
	// we need to not do
	// scanline 0 and 1 are (0 + 1) / 2.
	// scanline 1 and 2 are (1 + 2) / 2. 
		
    vec4 a = texture(tex0, texCoordVarying);
    float point = floor(mod(texCoordVarying.y,2.));
    vec2 p = vec2(0.,point);
    vec4 b = texture(tex0,texCoordVarying+p);
    vec4 c = texture(tex0,texCoordVarying-p);
    c = mix(b,c,0.5);
    c = mix(a,c,point);
    outputColor = c * opacity;
	
	// moo : short cow !
} 

