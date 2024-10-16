#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <glmm/glmm.h>

#include "shader.h"
#include "model.h"

#include <stdio.h>

#define MVP_LOCATION 0
#define MODEL_LOCATION 1
#define EYE_LOCATION 2
#define LIGHT_LOCATION 3


#define WIDTH 800
#define HEIGHT 600

int main(int argc, char** argv)
{
    /// Init SDL & OpenGL
    SDL_Init(SDL_INIT_VIDEO);

    // Telling SDL what we want to setup for opengl settings
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetSwapInterval(60);

    SDL_Window * window = SDL_CreateWindow("CGL", WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    gladLoadGL();

    // Setup Shader
    shader_t * shader = shader_create("passthru.vert.spv", "halfLambert.frag.spv");

    if (!shader)
    {
        printf("Freak out!\n");
    }

    model_t * m = model_load("");

    if (!m)
    {
        printf("Error no model\n");
    }

    shader_bind(shader);

    vec3f_t up = {0.0f, 1.0f, 0.0f};
    vec3f_t right = {1.0f, 0.0f, 0.0f};
    vec3f_t forward = {0.0f, 0.0f, 1.0f};


    vec3f_t lightPosition = {0.0f, 0.0f, -5.0f};

    vec3f_t eyePosition = {0.0f, 0.0f, -5.0f};

    mat4x4_t model;
    mat4x4_init(&model, 1.0f);

    mat4x4_t view;
    glmm_look_at(&view, &eyePosition, &(vec3f_t){0.0f, 0.0f, 0.0f}, &up);
    
    mat4x4_t projection;
    glmm_perspective(&projection, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, GLMM_RAD(45.0f));

    mat4x4_t mvp;
    glmm_mat4x4_xmul(&mvp, &projection, &view);
    glmm_mat4x4_mul(&mvp, &model);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get info for major/minor of opengl
    int major = 0;
    int minor = 0;

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("%d.%d\n", major, minor);

    // Game Loop
    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        mat4x4_rotate(&model, GLMM_RAD(0.25f), &up);
        mat4x4_rotate(&model, GLMM_RAD(0.25f), &right);
        mat4x4_rotate(&model, GLMM_RAD(0.25f), &forward);

        glmm_mat4x4_xmul(&mvp, &projection, &view);
        glmm_mat4x4_mul(&mvp, &model);

        glUniform3fv(EYE_LOCATION, 1, (GLfloat*)&eyePosition);
        glUniform3fv(LIGHT_LOCATION, 1, (GLfloat*)&lightPosition);

        glUniformMatrix4fv(MVP_LOCATION, 1, GL_FALSE, (GLfloat*)&mvp);
        glUniformMatrix4fv(MODEL_LOCATION, 1, GL_FALSE, (GLfloat*)&model);

        model_draw(m);

        SDL_GL_SwapWindow(window);
    }

    model_destroy(m);

    shader_destroy(shader);

    SDL_GL_DestroyContext(context);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}