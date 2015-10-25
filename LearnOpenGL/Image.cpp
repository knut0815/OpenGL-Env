#include "Image.h"

// ===============================
// Public member functions
// ===============================

Image::Image()
{
    
}

void Image::loadImage(const std::string &imagePath, int imageWidth, int imageHeight)
{
    width = imageWidth;
    height = imageHeight;
    pixelData = SOIL_load_image(imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);            // Subsequent commands will affect this texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    /*
     * The parameters of glTexImage2D are as follows:
     * 1) This operation will generate a texture on the currently bound texture object at the same target
     * 2) The mipmap level for which we want to create a texture
     * 3) The format that we want OpenGL to store our texture as
     * 4) The width of the resulting texture
     * 5) The height of the resulting texture
     * 6) Some legacy stuff: should always be 0
     * 7 / 8) The format and datatype of the source image
     * 9) The actual image data
     */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    glGenerateMipmap(GL_TEXTURE_2D);                    // Ask OpenGL to generate mipmaps for us
    
    //SOIL_free_image_data(pixelData);                  // We'd generally want this here...
    glBindTexture(GL_TEXTURE_2D, 0);                    // Unbind the texture object (best practice)
}

void Image::bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Image::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}