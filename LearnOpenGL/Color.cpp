#include "Color.h"

Color::Color()
{
    r = g = b = a = 255.0f;
}

Color::Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha)
{
    
}

void Color::set(float red, float green, float blue, float alpha)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
}
