#version 410

out vec4 frag_color;

in vec4 colorF;

void main()
{
	vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
	if (dot(circCoord, circCoord) > 1.0) {
	    discard;
	}
    frag_color = vec4(colorF.rgb,1.0);
}
