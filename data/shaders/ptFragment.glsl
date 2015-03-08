#version 410

out vec4 frag_color;

in vec3 colorF;
void main()
{
    frag_color = vec4(colorF,1.0);
}
