#include "model.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>
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
    FILE * fileptr = fopen(filename, "rt");

    if (!fileptr)
    {
        printf("File not found. %s\n", filename);
        return NULL;
    }

    fseek(fileptr, 0, SEEK_END);
    long size = ftell(fileptr);
    fseek(fileptr, 0, SEEK_SET);

    char * buffer = malloc(size + 1);
    buffer[size] = '\0';

    fread(buffer, 1, size, fileptr);

    size_t texcoordsCount = 0;
    size_t texcoordsCapacity = 100;
    vec2f_t * texcoords = calloc(texcoordsCapacity, sizeof(vec2f_t));

    size_t normalsCount = 0;
    size_t normalsCapacity = 100;
    vec4f_t * normals = calloc(normalsCapacity, sizeof(vec4f_t));

    size_t positionsCount = 0;
    size_t positionsCapacity = 100;
    vec4f_t * positions = calloc(positionsCapacity, sizeof(vec4f_t));

    size_t verticesCount = 0;
    size_t verticesCapacity = 99;
    vertex_t * vertices = calloc(verticesCapacity, sizeof(vertex_t));




    char * line = strtok(buffer, "\n");

    while (line)
    {
        if (line[0] == '#' || line[0] == '\0')
        {
            line = strtok(NULL, "\n");
            continue;
        }

        if (line[0] == 'v')
        {
            if (line[1] == 't')
            {
                if (texcoordsCount == texcoordsCapacity)
                {
                    texcoordsCapacity *= 2;
                    texcoords = realloc(texcoords, texcoordsCapacity * sizeof(vec2f_t));
                }

                int count = sscanf(line, "vt %f %f",
                    &texcoords[texcoordsCount].u,
                    &texcoords[texcoordsCount].v
                );

                if (count != 2)
                {
                    // freak out
                }

                texcoordsCount++;
            }
            else if (line[1] == 'n')
            {
                if (normalsCount == normalsCapacity)
                {
                    normalsCapacity *= 2;
                    normals = realloc(normals, normalsCapacity * sizeof(vec4f_t));
                }

                int count = sscanf(line, "vn %f %f %f",
                    &normals[normalsCount].x,
                    &normals[normalsCount].y,
                    &normals[normalsCount].z
                );

                normals[normalsCount].w = 1.0f;
                
                if (count != 3)
                {
                    // freak out
                }

                normalsCount++;
            }
            else {

                if (positionsCount == positionsCapacity)
                {
                    positionsCapacity *= 2;
                    positions = realloc(positions, positionsCapacity * sizeof(vec4f_t));
                }

                int count = sscanf(line, "v %f %f %f",
                    &positions[positionsCount].x,
                    &positions[positionsCount].y,
                    &positions[positionsCount].z
                );

                positions[positionsCount].w = 1.0f;

                if (count != 3)
                {
                    // freak out
                }

                positionsCount++;
            }
        }
        else if (line[0] == 'f')
        {   
            int positionIndices[3] = {0, 0, 0};
            int texcoordIndices[3] = {0, 0, 0};
            int normalIndices[3] = {0, 0, 0};

            if (strchr(line, '/'))
            {
                if (strstr(line, "//"))
                {
                    // position & normal
                    sscanf(line, "f %d//%d %d//%d %d//%d",
                        &positionIndices[0],
                        &normalIndices[0],
                        &positionIndices[1],
                        &normalIndices[1],
                        &positionIndices[2],
                        &normalIndices[2]
                    );
                }
                else {
                    // all 3
                    sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &positionIndices[0],
                        &texcoordIndices[0],
                        &normalIndices[0],
                        &positionIndices[1],
                        &texcoordIndices[1],
                        &normalIndices[1],
                        &positionIndices[2],
                        &texcoordIndices[2],
                        &normalIndices[2]
                    );

                    printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n", 
                        positionIndices[0],
                        texcoordIndices[0],
                        normalIndices[0],
                        positionIndices[1],
                        texcoordIndices[1],
                        normalIndices[1],
                        positionIndices[2],
                        texcoordIndices[2],
                        normalIndices[2]
                    );
                }
            }
            else {
                // positions only
                sscanf(line, "f %d %d %d",
                    &positionIndices[0],
                    &positionIndices[1],
                    &positionIndices[2]
                );
            }

            if (verticesCount + 2 >= verticesCapacity)
            {
                verticesCapacity *= 2;
                vertices = realloc(vertices, verticesCapacity * sizeof(vertex_t));
            }

            for (int i = 0; i < 3; ++i)
            {
                if (positionIndices[i] < 0)
                {
                    positionIndices[i] += positionsCount + 1;
                }

                vec4f_copy(&vertices[verticesCount + i].position, &positions[positionIndices[i] - 1]);

                if (texcoordIndices[i] != 0)
                {
                    if (texcoordIndices[i] < 0)
                    {
                        texcoordIndices[i] += texcoordsCount + 1;
                    }

                    vec2f_copy(&vertices[verticesCount + i].texcoord, &texcoords[texcoordIndices[i] - 1]);
                }

                if (normalIndices[i] != 0)
                {
                    if (normalIndices[i] < 0)
                    {
                        normalIndices[i] += normalsCount + 1;
                    }
                    
                    vec4f_copy(&vertices[verticesCount + i].normal, &normals[normalIndices[i] - 1]);
                }
            }

            verticesCount += 3;
        }
        else if (strncmp(line, "mtllib ", sizeof("mtllib ") - 1) == 0)
        {
            
        }
        else if (strncmp(line, "usemtl ", sizeof("usemtl ") - 1) == 0)
        {
            
        }

        line = strtok(NULL, "\n");
    }

    for (int i = 0; i < verticesCount; ++i)
    {
        vec4f_print(&vertices[i].position);
    }

    fclose(fileptr);

    model_t * model = calloc(1, sizeof(model_t));

    glGenVertexArrays(1, &model->VAO);

    glGenBuffers(1, &model->VBO);

    glBindVertexArray(model->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(vertex_t), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texcoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    model->count = verticesCount;

    free(vertices);
    free(positions);
    free(normals);
    free(texcoords);
    free(buffer);

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