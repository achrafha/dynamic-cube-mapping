#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <GL/glu.h>
#include <SDL.h>

#include "state.h"

#define WINDOW_TITLE "dynamic-cube-mapping"

struct {
  struct graphicsState graphics;
  GLuint texture;
} state;

void initialize(int argc, char **argv);
void mainloop(void);
void cleanup(void);

int main(int argc, char **argv) {
  initialize(argc, argv);
  mainloop();
  cleanup();
  return EXIT_SUCCESS;
}

void initConfig(int argc, char **argv);
void initWindow(void);
void initContext(void);
void initResources(void);

void initialize(int argc, char **argv) {
  initConfig(argc, argv);
  initWindow();
  initContext();
  initResources();
}

void initConfig(int argc, char **argv) {
  if (argc != 1) {
    fprintf(stderr, "Usage: %s\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

void initResources() {
  SDL_Surface *surface = SDL_LoadBMP("lenna.bmp");
  glGenTextures(1, &(state.texture));
  glBindTexture(GL_TEXTURE_2D, state.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  SDL_FreeSurface(surface);
}

void rendering(void);
int keyboard(SDL_Event *event);
void resize(int w, int h, struct graphicsState *gs);

void mainloop() {
  int continuer = 1;
  SDL_Event event;
  
  // Boucle traitant les évènements de la fenêtre
  while (continuer) {
    // On attend le prochain évènement
    SDL_WaitEvent(&event);
    // On traite l'évènement
    switch (event.type) {
    case SDL_QUIT:
      // On a fermé la fenetre
      continuer = 0;
      break;
    case SDL_KEYDOWN:
      // On a appuyé sur une touche
      continuer = keyboard(&event);
      break;
    case SDL_WINDOWEVENT:
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        resize(event.window.data1, event.window.data2, &state.graphics);
        break;
      }
      break;
    }
    rendering();
  }
}

void cleanup() { 
  SDL_SetWindowFullscreen(state.graphics.window, 0);
  SDL_GL_DeleteContext(state.graphics.context);
  SDL_DestroyWindow(state.graphics.window);
  SDL_Quit();
}

/**
 * Initialisation de la SDL, création du contexte OpenGL et ouverture de la fenetre.
 */
void initWindow(void) {
  if (SDL_VideoInit(NULL) < 0) {
    fprintf(stderr, "Erreur à l'initialisation de la vidéo : %s\n",
            SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  
  state.graphics.window = SDL_CreateWindow(WINDOW_TITLE,
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             640, 640,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  
  if (state.graphics.window == 0) {
    fprintf(stderr, "Erreur lors de la création de la fenêtre (SDL)\n");
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  
  state.graphics.context = SDL_GL_CreateContext(state.graphics.window);
  if (state.graphics.context == 0) {
    fprintf(stderr, "Erreur lors de la création du contexte OpenGL\n");
    SDL_DestroyWindow(state.graphics.window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  
}

/**
 * Initialize OpenGL Context
 */
void initContext(void) {
  fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * Catch OpenGL errors
 */
void catchGLError(void) {
  GLenum openGLError;
  if ((openGLError = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "Erreur OpenGL dans display : %s\n", gluErrorString(openGLError));
  }
}

/**
 * Scene display
 */
void rendering(void) {
  SDL_GL_SwapWindow(state.graphics.window);
  catchGLError();
}


/**
 * Keyboard input
 * @param event SDL event that triggered the function call
 * (must have type SDL_KEYDOWN).
 */
int keyboard(SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    switch(event->key.keysym.sym) {
    case SDLK_ESCAPE:
      return 0;
    case SDLK_RETURN:
      if (!state.graphics.fullscreen) {
        if (SDL_SetWindowFullscreen(state.graphics.window, SDL_WINDOW_FULLSCREEN) == 0) {
          state.graphics.fullscreen = true;
        } else {
          fprintf(stderr, "Erreur lors du passage au plein au écran : %s\n",
                  SDL_GetError());
        }
      } else {
        if (SDL_SetWindowFullscreen(state.graphics.window, 0) == 0) {
          state.graphics.fullscreen = false;
        } else {
          fprintf(stderr, "Erreur lors du retour au mode fenêtré : %s\n",
                  SDL_GetError());          
        }
      }
      break;
    default:
      break;
    }
  }
  
  return 1;
}

/**
 * Resizes the OpenGL context
 */
void resize(int w, int h, struct graphicsState *gs) {
  gs->window_width = w;
  gs->window_height = h;
  glViewport(0, 0, w, h);
}
