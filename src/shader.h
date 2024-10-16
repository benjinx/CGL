#ifndef SHADER_H
#define SHADER_H

typedef struct shader shader_t;


shader_t * shader_create(const char * vertexFilename, const char * fragmentFilename);

void shader_destroy(shader_t * shader);

void shader_bind(shader_t * shader);

#endif // SHADER_H