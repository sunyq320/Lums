/* ************************************************************************** */
/*                                                                            */
/*                                                  &&&&&&       &&&&&&       */
/*    CoreGL.cpp                                   &------&     &------&      */
/*                                                  &&-----&   &-----&&       */
/*                                                    &&&&#######&&&&         */
/*                                                       #.......#            */
/*                                                       #.....  #            */
/*    This file is part of the                           #...    #            */
/*    Lums library.                                       #######             */
/*                                                                            */
/* ************************************************************************** */

#include <Lums/CoreGL.h>

using namespace lm;

CoreGL::CoreGL(int w, int h, const char* name, bool fullscreen) : _width(w),
                                                                  _height(h)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    _win = SDL_CreateWindow(name,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            w,
                            h,
                            SDL_WINDOW_OPENGL | (fullscreen
                                                 ? SDL_WINDOW_FULLSCREEN
                                                 : 0));
    _glcontext = SDL_GL_CreateContext(_win);
    glClearColor(0, 0, 0, 1);
}

CoreGL::~CoreGL()
{
    SDL_GL_DeleteContext(_glcontext);
    SDL_DestroyWindow(_win);
    SDL_Quit();
}