#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texcoord;

out vec2 TexCoord;

void main()
{
   gl_Position = vec4(position.x, position.y, -position.z, position.w);
   TexCoord = texcoord;
}
