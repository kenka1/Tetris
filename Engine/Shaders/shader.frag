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
        if(fColor.x > 0.8 && fColor.y > 0.8 && fColor.z > 0.8)
            discard;
        fColor = mix(fColor, uColor, 0.8);
    }
    else
        fColor = uColor;
}