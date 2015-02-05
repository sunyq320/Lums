/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    Nyan.cpp                                       oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#include <Lums/Lums.hpp>

class Nyan : public lm::GameState
{
public:
    Nyan()
    : _linear(true)
    {
        glEnable(GL_TEXTURE_2D);
        _image.loadFile("Nyan.png");
        _sprite.setImage(_image);
        _sprite.setSize(2, 2);
    }

    void
    update()
    {

    }

    void
    handleEvent(const lm::Event& event)
    {
        if (event.type == lm::Event::Type::KeyDown)
        {
            if (event.key == lm::Key::V)
            {
                _linear = !_linear;
                _image.linear(_linear);
            }
            else
                lm::Core::get().stop();
        }
    }

    void
    render() const
    {
        _sprite.draw(-1, -1);
    }

private:
    lm::Image   _image;
    lm::Sprite  _sprite;
    bool        _linear;
};

int
main()
{
    lm::Core core(400, 400, "Nyan");

    core.push<Nyan>();
    core.start();
}