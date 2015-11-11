#ifndef __LearnOpenGL__color__
#define __LearnOpenGL__color__

// This should really be a templated class...see Cinder/include/Color.h

class Color
{
    
public:
    
    Color();
    Color(float red, float green, float blue, float alpha = 255.0f);
    void set(float red, float green, float blue, float alpha);
    float getRed() const { return r; }
    float getGreen() const { return g; }
    float getBlue() const { return b; }
    float getAlpha() const { return a; }

private:
    
    float r;
    float g;
    float b;
    float a;
    
};
#endif
