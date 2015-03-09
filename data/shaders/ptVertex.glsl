#version 330 

uniform mat4 projectionview;

in vec4 position;
in vec4 color;

out vec4 colorF;

void main()
{ 
	gl_Position = projectionview*vec4(position.xyz,1.0);
	colorF = color;
}  