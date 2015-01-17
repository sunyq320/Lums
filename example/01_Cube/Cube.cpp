#include "Cube.h"
#include <iostream>

using namespace lm;

const GLfloat   vertices[] = {
     1,  1,  1,
     1,  1, -1,
     1, -1,  1,
    -1,  1,  1,
     1, -1, -1,
    -1,  1, -1,
    -1, -1,  1,
    -1, -1, -1
};

const GLfloat   colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0,
    1, 0, 1,
    0, 1, 1,
    1, 0, 1,
    1, 1, 1
};

const GLubyte   indices[] = {
    0, 2, 6, 3,
    3, 5, 7, 6,
    5, 1, 4, 7,
    1, 0, 2, 4,
    1, 0, 3, 5,
    2, 4, 7, 6
};

Cube::Cube(lm::Core* core) : lm::GameState(core), speed(10, 0)
{
    
}

void    Cube::load()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPerspective(70, 1.333f, 0.001f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void    Cube::update()
{
    angle.x += 0.1_deg * speed.x;
    angle.y += 0.1_deg * speed.y;
}

void    Cube::handleEvent(const Event& event)
{
    if (event.type == Event::Type::KeyDown)
    {
        switch (event.key)
        {
            case Key::Escape:
                Core().stop();
                break;
            case Key::Right:
                speed.x++;
                break;
            case Key::Left:
                speed.x--;
                break;
            case Key::Up:
                speed.y++;
                break;
            case Key::Down:
                speed.y--;
                break;
            default:
                break;
        }
    }
}

void    Cube::render()
{
    glLookAt(-2, 2, -2, 0, 0, 0, 0, 1, 0);
    glRotatef(angle.x.toDegrees(), 0, 1, 0);
    glRotatef(angle.y.toDegrees(), 0, 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

}

void    Cube::unload()
{
    glDisable(GL_DEPTH_TEST);
}