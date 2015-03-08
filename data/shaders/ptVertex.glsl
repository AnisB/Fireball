#version 330 

uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 color;

out vec3 colorF;

void main()
{ 
	gl_Position = projection*view*vec4(position,1.0);
	colorF = color;
}  