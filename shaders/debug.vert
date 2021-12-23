#version 450 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 VP;

void main()
{
    gl_Position = VP * instanceMatrix * vec4(aPos, 1.0f);
}