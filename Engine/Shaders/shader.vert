#version 460 core

layout (location = 0) in vec3 vPosition;

uniform mat4 Transform;

void main()
{
    gl_Position = Transform * vec4(vPosition, 1.0f);
}