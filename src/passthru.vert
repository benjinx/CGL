#version 450 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aTexcoord;

layout (location = 0) uniform mat4 uMVP;
layout (location = 1) uniform mat4 uModel;

layout (location = 0) out vec3 vNormal;
layout (location = 1) out vec2 vTexcoord;
layout (location = 2) out vec3 vPosition;
layout (location = 3) out vec3 vNormalOG;

void main()
{
    vPosition = mat3(uModel) * aPosition.xyz;

    gl_Position = uMVP * aPosition;

    vNormal = mat3(transpose(inverse(uModel))) * aNormal.xyz;

    vNormalOG = aNormal.xyz;

    vTexcoord = aTexcoord;
}