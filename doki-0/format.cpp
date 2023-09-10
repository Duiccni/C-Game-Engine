/*
Codded with "C:\doki\doki\version_info.txt", SDL 2.26.1 and OpenGL in c++
"C:\Program Files\LLVM\bin\clang-format.exe" "C:\doki\doki\main.cpp"
Compiler MinGW32 g++
By Egemen Yalın
*/

// default c++ librarys
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

// alone poor windows :<
#include <windows.h> // Sleep(milliseconds);

// defining sdl and OpenGL librarys
#define SDL_MAIN_HANDLED
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// defining sdl_util librarys
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/*
C:\Windows\System32\opengl32.dll
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu.lib")
*/

using namespace std;
using namespace chrono;

const high_resolution_clock::time_point tstart = high_resolution_clock::now();

int main(int argc, char *argv[]) {
  system("systeminfo");

  // defining game basic params
  const int tilesize = 32, cols = 32, rows = 18, fps = 60,
            WIDTH = tilesize * cols, HEIGHT = tilesize * rows;
  const float tsleep = 1000 / fps;
  float tdelta = 1 / fps, rfps = fps;
  int tick = NULL;
  char state = 's';

  // initialise sdl librarys and check errors
  if (SDL_Init(SDL_INIT_EVERYTHING) < EXIT_SUCCESS) {
    cerr << "\nFailed to initialise sdl, Simple DirectMedia Layer.";
    return EXIT_FAILURE;
  }
  if (TTF_Init() < EXIT_SUCCESS) {
    cerr << "\nFailed to initialise ttf library, TrueType Font.";
    SDL_Quit();
    return EXIT_FAILURE;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < EXIT_SUCCESS) {
    cerr << "\nFailed to initialise Mixer.";
    SDL_Quit();
    TTF_Quit();
    return EXIT_FAILURE;
  }
  if (IMG_Init(IMG_INIT_JPG) < EXIT_SUCCESS) {
    cerr << "\nFailed to initialise Image library.";
    // looking shitta bad so much repeat
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
    return EXIT_FAILURE;
  }
  cout << "\nSuccessfully initialised all librarys.\n";

  // creating window and initialise OpenGL
  SDL_Window *window = SDL_CreateWindow("loftl", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                        SDL_WINDOW_OPENGL);
  if (window == NULL) {
    cerr << "\nFailed to create window.";
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    return EXIT_FAILURE;
  }
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    cerr << "\nFailed to initialise OpenGL Glew.";
    // looking shitta bad so much repeat
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    return EXIT_FAILURE;
  }
  glClearColor(.0f, .0f, .0f, 1.0f);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  bool going = true, debug = false, cache;
  int mousex, mousey, qtick = NULL, ttick;

  const SDL_Color sdlclrs[] = {{255, 255, 255}};
  TTF_Font *sfont = TTF_OpenFont("C:/doki/doki/bin/VT323.ttf", 72);

  Sleep(500);
  if (sfont == NULL) {
    cerr << "\nFailed to read font.";
    // looking shitta bad so much repeat
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    return EXIT_FAILURE;
  }

  // OpenGL things even i dont know whats happening
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(.0f, WIDTH, HEIGHT, .0f, -1.0f, 1.0f);

  SDL_Surface *brued;
  GLuint texture;
  SDL_Event e;

  while (going) {
    ttick = SDL_GetTicks();
    SDL_GetMouseState(&mousex, &mousey);
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT ||
          (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        if (qtick + fps > tick) {
          going = false;
        } else {
          qtick = tick;
        }
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_QUOTEDBL:
          cache = true;
          debug ^= true;
          break;
        }
      }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (debug) {
      if (tick % fps == 0 || cache == true) {
        cache = false;
        brued = TTF_RenderText_Blended(sfont, "abcdefg", sdlclrs[0]);
      }

      // OpenGL things (rendering texture of font with text)
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brued->w, brued->h, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, brued->pixels);

      // Drawing
      glBegin(GL_QUADS);
      glTexCoord2i(0, 0);
      glVertex2i(0, 0);
      glTexCoord2i(1, 0);
      glVertex2i(brued->w, 0);
      glTexCoord2i(1, 1);
      glVertex2i(brued->w, brued->h);
      glTexCoord2i(0, 1);
      glVertex2i(0, brued->h);
      glEnd();
    }

    SDL_GL_SwapWindow(window);
    SDL_Delay(tsleep);
    tdelta = (float)(SDL_GetTicks() - ttick) / 1000;
    rfps = 1 / tdelta;
    tick++;
  }

  // destroying structs and printing process time
  SDL_FreeSurface(brued);
  TTF_CloseFont(sfont);
  // OpenGL shit
  glDeleteTextures(1, &texture);
  // destroy window
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  // looking shitta bad so much repeat
  SDL_Quit();
  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  milliseconds durc =
      duration_cast<milliseconds>(high_resolution_clock::now() - tstart);
  cout << "\nProcess completed in " << durc.count() << "ms.";
  return EXIT_SUCCESS;
}