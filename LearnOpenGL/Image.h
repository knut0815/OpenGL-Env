#ifndef __LearnOpenGL__image__
#define __LearnOpenGL__image__

#include <string>
#include <GL/glew.h>
#include <SOIL/SOIL.h>

class Image
{
    
public:
    
    Image();
    void loadImage(const std::string &imagePath, int imageWidth, int imageHeight);
    void bind() const;
    void unbind() const;
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline unsigned char* getPixelData() const { return pixelData; }
    inline void clearPixelData() { if (pixelData) SOIL_free_image_data(pixelData); }
    inline GLuint getTextureRef() const { return textureID; }
    
private:
    
    int width;
    int height;
    unsigned char* pixelData;
    GLuint textureID;
    
};

#endif
