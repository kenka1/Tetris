#version 460 core

in vec2 TexCoord;

uniform vec4 uColor;
uniform int isTexture;
uniform sampler2D uTexture;

out vec4 fColor;

void main()
{
    if(isTexture == 1)
    {
        fColor = texture(uTexture, TexCoord);
        const float treshold = 0.91f;
        if(fColor.x > treshold && fColor.y > treshold && fColor.z > treshold)
            discard;
        fColor = mix(fColor, uColor, 0.5);
    }
    else
        fColor = uColor;
}