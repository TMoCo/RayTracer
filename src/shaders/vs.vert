#version 450 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec2 texCoord; // specify a color output to the fragment shader

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    texCoord = vec2(1.0f, 1.0f);
}