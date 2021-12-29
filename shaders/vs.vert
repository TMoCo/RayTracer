#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;

out vec2 uv; // specify a color output to the fragment shader
out vec3 normal;

uniform mat4 PVM;

void main()
{
    gl_Position = PVM * vec4(aPos, 1.0f);
    normal = aNor;
    uv = aTex;
}