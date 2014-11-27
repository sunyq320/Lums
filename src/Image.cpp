/* ************************************************************************** */
/*                                                                            */
/*                                                  &&&&&&       &&&&&&       */
/*    Image.cpp                                    &------&     &------&      */
/*                                                  &&-----&   &-----&&       */
/*                                                    &&&&#######&&&&         */
/*                                                       #.......#            */
/*                                                       #.....  #            */
/*    This file is part of the                           #...    #            */
/*    Lums library.                                       #######             */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <Lums/Image.h>
#include <Lums/OperatingSystem.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace lm;

Image::Image()
: _width(0), _height(0), _texture(0)
{

}

Image::Image(Image&& rhs)
: _width(rhs._width), _height(rhs._height), _texture(rhs._texture)
{
    rhs._texture = 0;
}

Image&
Image::operator=(Image&& rhs)
{
    _width = rhs._width;
    _height = rhs._height;
    _texture = rhs._texture;
    rhs._texture = 0;
    return *this;
}

void
Image::LoadFile(const std::string path, bool resource)
{
    std::string     file = resource ? resourcePath() + path : path;
    SDL_Surface*    tmp;

    if (_texture)
        glDeleteTextures(1, &_texture);
    tmp = IMG_Load(file.c_str());
    _width = tmp->w;
    _height = tmp->h;
    Gen(tmp);
    SDL_FreeSurface(tmp);
}

Image
Image::FromFile(const std::string path)
{
    Image   a;

    a.LoadFile(path);
    return a;
}

void
Image::Gen(SDL_Surface* surface)
{
    int             buf;
    int             w = _width;
    SDL_Surface*    image;

    image = SDL_CreateRGBSurface(0, _width, _height, 32,
                                 0xff, 0xff00, 0xff0000, 0xff000000);
    SDL_BlitSurface(surface, 0, image, 0);

    // We need to flip the image, because the crappy SDL_Image loader
    // stores them upside down, for compatibility with the even more
    // crappy SDL BMP Loader.
    for (int j = 0; j < image->h / 2; j++)
    {
        for (int i = 0; i < w; i++)
        {
            buf = ((int*)image->pixels)[j * w + i];
            ((int*)image->pixels)[j * w + i] = ((int*)image->pixels)[(image->h - 1 - j) * w + i];
            ((int*)image->pixels)[(image->h - 1 - j) * w + i] = buf;
        }
    }
    glGenTextures(1, &_texture);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
    SDL_FreeSurface(image);
}

Image::~Image()
{
    if (_texture)
        glDeleteTextures(1, &_texture);
}
