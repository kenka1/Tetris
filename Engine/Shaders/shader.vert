#version 460 core

layout (location = 0) in vec2 vPosition;

uniform mat4 uTransform;
uniform int uGrid_ID;

out vec2 TexCoord;

void main()
{
    vec4 offset = vec4(gl_InstanceID * 50.0f, 0.0f, 0.0f, 0.0f);
    if(uGrid_ID == 1)
        offset = vec4(0.0, gl_InstanceID * 50.0f, 0.0f, 0.0f);

    gl_Position = uTransform * (vec4(vPosition, 0.0f, 1.0f) + offset);
    TexCoord = vPosition;
}