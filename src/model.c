#include "model.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <glmm/glmm.h>
#include <assert.h>

typedef struct model
{
    GLuint VAO;
    GLuint VBO;
    unsigned count;
} model_t;

typedef struct vertex
{
    vec4f_t position;
    vec4f_t normal;
    vec2f_t texcoord;
} vertex_t;

// Packing @ size 40
static_assert(sizeof(vertex_t) == 40, "sizeof(vertex_t) != 40");

model_t * model_load(const char * filename)
{
    // Actually load model
    vertex_t vertices[] = {
        {
            { 1.0f, -1.0f, 1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 1.0f, 0.333333f },
        },
        {
            { -1.0f, -1.0f, 1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 1.0f, 0.666667f },
        },
        {
            { -1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.666667f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 1.0f, 0.333333f },
        },
        {
            { -1.0f, 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.333333f },
        },
        {
            { 0.999999f, 1.0f, 1.000001f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.0f },
        },
        {
            { 1.0f, 1.0f, -0.999999f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.333333f },
        },
        {
            { 0.999999f, 1.0f, 1.000001f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f },
        },
        {
            { 1.0f, -1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.333333f, 0.0f },
        },
        {
            { 0.999999f, 1.0f, 1.000001f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.333333f, 0.0f },
        },
        {
            { -1.0f, 1.0f, 1.0f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.666667f, 0.0f },
        },
        {
            { -1.0f, -1.0f, 1.0f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.666667f, 0.333333f },
        },
        {
            { -1.0f, -1.0f, 1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.333333f, 1.0f },
        },
        {
            { -1.0f, 1.0f, 1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.0f, 1.0f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.0f, 0.666667f },
        },
        {
            { 1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.333333f, 0.333333f },
        },
        {
            { -1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.333333f, 0.666667f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.0f, 0.666667f },
        },
        {
            { 1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.333333f },
        },
        {
            { 1.0f, -1.0f, 1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 1.0f, 0.333333f },
        },
        {
            { -1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, -1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.666667f },
        },
        {
            { 1.0f, 1.0f, -0.999999f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 1.0f, 0.0f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 1.0f, 0.333333f },
        },
        {
            { 0.999999f, 1.0f, 1.000001f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.666667f, 0.0f },
        },
        {
            { 1.0f, -1.0f, -1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.333333f, 0.333333f },
        },
        {
            { 1.0f, 1.0f, -0.999999f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.333333f },
        },
        {
            { 1.0f, -1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.333333f, 0.0f },
        },
        {
            { 1.0f, -1.0f, 1.0f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.333333f, 0.333333f },
        },
        {
            { 0.999999f, 1.0f, 1.000001f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.333333f, 0.0f },
        },
        {
            { -1.0f, -1.0f, 1.0f, 1.0f },
            { -0.0f, 0.0f, 1.0f, 1.0f },
            { 0.666667f, 0.333333f },
        },
        {
            { -1.0f, -1.0f, -1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.333333f, 0.666667f },
        },
        {
            { -1.0f, -1.0f, 1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.333333f, 1.0f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { -1.0f, -0.0f, -0.0f, 1.0f },
            { 0.0f, 0.666667f },
        },
        {
            { 1.0f, 1.0f, -0.999999f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.0f, 0.333333f },
        },
        {
            { 1.0f, -1.0f, -1.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.333333f, 0.333333f },
        },
        {
            { -1.0f, 1.0f, -1.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f, 1.0f },
            { 0.0f, 0.666667f },
        },
    };

    model_t * model = calloc(1, sizeof(model_t));

    glGenVertexArrays(1, &model->VAO);

    glGenBuffers(1, &model->VBO);

    glBindVertexArray(model->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texcoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    model->count = 36;

    return model;
}

void model_destroy(model_t * model)
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &model->VBO);
    glDeleteVertexArrays(1, &model->VAO);

    free(model);
}

void model_draw(model_t * model)
{
    // Draw
    glBindVertexArray(model->VAO);
    glDrawArrays(GL_TRIANGLES, 0, model->count);
}