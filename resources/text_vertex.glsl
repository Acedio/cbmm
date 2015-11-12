#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 transform = mat4(1.0);

out vec2 TexCoord;

void main()
{
   gl_Position = transform * vec4(position.x, position.y, 0, 1);
   TexCoord = texcoord;
}
