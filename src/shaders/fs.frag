#version 450 core
out vec4 FragColor;
  
in vec2 texCoord; // the input variable from the vertex shader (same name and same type)  

// uniform sampler2D textureSampler;

void main()
{
    // FragColor = texture(textureSampler, texCoord);
    FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}