#version 450 core
in vec2 texCoord; // the input variable from the vertex shader (same name and same type)  
in vec3 normal;

out vec4 FragColor;

// uniform sampler2D textureSampler;

void main()
{
    FragColor = vec4( 0.5f * (vec3(1.0f) + normal), 1.0f);
}