#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <gl/GL.h>

typedef struct shader
{
    GLuint id;
} shader_t;

GLuint _shader_load(shader_t * shader, GLenum stage, const char * filename)
{
    FILE * fileptr = fopen(filename, "rb");

    // Seek to the end of the file, get the size, then seek back
    fseek(fileptr, 0, SEEK_END);
    long size = ftell(fileptr);
    fseek(fileptr, 0, SEEK_SET);

    // make a buffer with our newly found size
    char * buffer = malloc(size);

    // Read the buffer in
    (void)fread(buffer, 1, size, fileptr);

    fclose(fileptr);
    
    GLuint id = glCreateShader(stage);
    glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer, size);
    glSpecializeShader(id, "main", 0, 0, 0);

    free(buffer);

    GLint compiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        printf("Failed to compile shader: %s\n", filename);

        char infoLog[512];
        glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
        printf("%s\n", infoLog);

        return 0;
    }

    glAttachShader(shader->id, id);

    return id;
}

shader_t * shader_create(const char * vertexFilename, const char * fragmentFilename)
{
    // 'malloc' pointer
    shader_t * shader = calloc(1, sizeof(shader_t));

    shader->id = glCreateProgram();

    GLuint vertexID = _shader_load(shader, GL_VERTEX_SHADER, vertexFilename);

    GLuint fragmentID = _shader_load(shader, GL_FRAGMENT_SHADER, fragmentFilename);

    int success = 0;
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        printf("Failed to link status\n");

        char infoLog[512];
        glGetProgramInfoLog(shader->id, sizeof(infoLog), NULL, infoLog);
        printf("%s\n", infoLog);
    }

    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);

    return shader;
}

void shader_destroy(shader_t * shader)
{
    glDeleteProgram(shader->id);
    free(shader);
}

void shader_bind(shader_t * shader)
{
    glUseProgram(shader->id);
}