#include "Cube.h"

using namespace lm;

int main()
{
    Core  gl(800, 600, "Cube");

    gl.push<Cube>();
    gl.start();
}