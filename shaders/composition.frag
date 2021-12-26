#version 450 core
in vec2 uv; // the input variable from the vertex shader (same name and same type)  

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, uv);
    // FragColor = vec4(uv, 0.0f, 1.0f);
}