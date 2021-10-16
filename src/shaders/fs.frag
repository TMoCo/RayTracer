#version 450 core
out vec4 FragColor;
  
in vec2 texCoord; // the input variable from the vertex shader (same name and same type)  

uniform sampler2D textureSampler;

void main()
{
    FragColor = vec4(texCoord, 0.0f, 1.0f);
    FragColor = texture(textureSampler, texCoord);
}