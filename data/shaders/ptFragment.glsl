#version 410

out vec4 frag_color;

in vec4 colorF;

void main()
{
    frag_color = colorF;
}
