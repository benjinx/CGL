#ifndef MODEL_H
#define MODEL_H

typedef struct model model_t;


model_t * model_load(const char * filename);

void model_destroy(model_t * model);

void model_draw(model_t * model);

#endif // MODEL_H