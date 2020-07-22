#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define HIGH_QUALITY

#ifdef HIGH_QUALITY
    #define float double
    #define glUniform1f glUniform1d
    #define glUniform2f glUniform2d
    #define glUniform3f glUniform3d
    #define glUniform4f glUniform4d
    #define glUniform1fv glUniform1dv
    #define glUniform2fv glUniform2dv
    #define glUniform3fv glUniform3dv
    #define glUniform4fv glUniform4dv
#endif

extern SDL_Window *win;
extern GLuint program;

void mainpart(void) {
    // Get uniform locations
    GLuint uTime = glGetUniformLocation(program, "time");
    GLuint uResolution = glGetUniformLocation(program, "resolution");
    GLuint uMandelbrotCenter = glGetUniformLocation(program, "mandelbrotCenter");
    GLuint uMandelbrotSize = glGetUniformLocation(program, "mandelbrotSize");

    float mandelbrotCenterX = 0.;
    float mandelbrotCenterY = 0.;
    float mandelbrotSize  = 2.;

    // Mainloop
    SDL_Event e;
    float last_time = SDL_GetTicks() / 1000.;
    for(;;) {
        float time = SDL_GetTicks() / 1000.;
        float dtime = time - last_time;
        last_time = time;

        bool quit = 0;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        if (quit) break;

        // Set the time uniform
        glUniform1f(uTime, time);

        // Set the resolution uniform
        int W, H;
        SDL_GetWindowSize(win, &W, &H);
        glViewport(0, 0, W, H);
        glUniform2i(uResolution, W, H);

        // Set the mandelbrot uniforms
        const Uint8 *key = SDL_GetKeyboardState(NULL);
        float speedFactor = 1.;
        if (key[SDL_SCANCODE_LSHIFT])     speedFactor = 2.0;
        else if (key[SDL_SCANCODE_LCTRL]) speedFactor = 0.5;

        if (key[SDL_SCANCODE_W]) mandelbrotCenterY += dtime * speedFactor * mandelbrotSize * 0.8;
        if (key[SDL_SCANCODE_S]) mandelbrotCenterY -= dtime * speedFactor * mandelbrotSize * 0.8;
        if (key[SDL_SCANCODE_D]) mandelbrotCenterX += dtime * speedFactor * mandelbrotSize * 0.8;
        if (key[SDL_SCANCODE_A]) mandelbrotCenterX -= dtime * speedFactor * mandelbrotSize * 0.8;

        if (key[SDL_SCANCODE_E] | key[SDL_SCANCODE_EQUALS]) mandelbrotSize /= pow(5., dtime * speedFactor);
        if (key[SDL_SCANCODE_Q] | key[SDL_SCANCODE_MINUS])  mandelbrotSize *= pow(5., dtime * speedFactor);

        glUniform1f(uMandelbrotSize, mandelbrotSize);
        glUniform2f(uMandelbrotCenter, mandelbrotCenterX, mandelbrotCenterY);

        glClear(GL_COLOR_BUFFER_BIT);
        glRectf(-1., -1., 1., 1.);

        SDL_GL_SwapWindow(win);
    }
}
