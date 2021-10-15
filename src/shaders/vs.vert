#version 450 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;


out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = vec4(aCol, 1.0);
}