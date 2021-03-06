/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Font.cpp                                       oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <cstdio>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <LumsInclude/OperatingSystem.hpp>
#include <LumsInclude/Graphics/Font.hpp>
#include <LumsInclude/Graphics/Image.hpp>
#include <LumsInclude/Graphics/Graphics.hpp>

using namespace lm;

static FT_Library ftLibrary;

void
Freetype::init()
{
    FT_Init_FreeType(&ftLibrary);
}

Font::Font()
: _size(0.f)
{

}

void
Font::setPath(const std::string& path, bool resource)
{
    _path = resource ? resourcePath() + '/' + path : path;
}

void
Font::setSize(float size)
{
    _size = size;
}

void
Font::load()
{
    Image img;
    FT_Face face;
    FT_GlyphSlot glyph;
    unsigned char* bitmap[glyphCount];
    int bitmapSize = 256;
    unsigned char* buffer;
    int left[glyphCount];
    int top[glyphCount];

    float size = _size * globalScale();
    img.setScale(1.f / globalScale());

    FT_New_Face(ftLibrary, _path.c_str(), 0, &face);    
    FT_Set_Char_Size(face, 0, size * 64, 0, 0);
    glyph = face->glyph;
    
    for (int i = 0; i < glyphCount; ++i)
    {
        FT_Vector kern;
        FT_Load_Char(face, i, FT_LOAD_RENDER);
        bitmap[i] = new unsigned char[glyph->bitmap.width * glyph->bitmap.rows];
        _glyphs[i].width = glyph->bitmap.width;
        _glyphs[i].height = glyph->bitmap.rows;
        _glyphs[i].left = glyph->bitmap_left;
        _glyphs[i].top = glyph->bitmap_top;
        _glyphs[i].advance = float(glyph->advance.x) / 64.f * (1.f / globalScale());
        std::memcpy(bitmap[i], glyph->bitmap.buffer, glyph->bitmap.width * glyph->bitmap.rows);
        for (int j = 0; j < glyphCount; ++j)
        {
            FT_Get_Kerning(face, i, j, 0, &kern);
            _glyphs[i].kerning[j] = kern.x / 64.f;
        }
    }

    layoutGlyphs:
    int actualX = 0;
    int actualY = 0;
    int maxY = 0;
    for (int i = 0; i < glyphCount; ++i)
    {
        if (actualX + _glyphs[i].width >= bitmapSize)
        {
            actualX = 0;
            actualY += maxY;
            maxY = 0;
        }
        if (actualY + _glyphs[i].height >= bitmapSize)
        {
            bitmapSize += 128;
            goto layoutGlyphs;
        }
        if (_glyphs[i].height > maxY)
            maxY = _glyphs[i].height;
        left[i] = actualX;
        top[i] = actualY;
        actualX += _glyphs[i].width;
    }

    _texture.resetAtlas();
    buffer = new unsigned char[bitmapSize * bitmapSize * 4];
    std::memset(buffer, 0, bitmapSize * bitmapSize * 4);
    for (int idx = 0; idx < glyphCount; ++idx)
    {
        for (int j = 0; j < _glyphs[idx].height; ++j)
        {
            for (int i = 0; i < _glyphs[idx].width; ++i)
            {
                std::memset(buffer + (left[idx] + i) * 4 + (top[idx] + j) * 4 * bitmapSize, bitmap[idx][i + _glyphs[idx].width * j], 4);
            }
        }
        _texture.pushAtlas({{float(left[idx]) / bitmapSize, float(top[idx]) / bitmapSize}, {float(_glyphs[idx].width) / bitmapSize, float(_glyphs[idx].height) / bitmapSize}});
        delete [] bitmap[idx];
    }
    img.setBuffer(buffer, bitmapSize, bitmapSize, GL_RGBA, 1.f / globalScale());
    _texture.setImage(img);
    _texture.load();
}

void
Font::unload()
{
    _texture.unload();
}

bool
Font::loaded() const
{
    return _texture.loaded();
}

void
Font::loadBinary(const BObject& object)
{
    setPath(object["path"].asString());
    setSize(object["size"].asFloat());
}

Font::~Font()
{
    unload();
}
