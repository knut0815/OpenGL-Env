#ifndef __LearnOpenGL__glslProgram__
#define __LearnOpenGL__glslProgram__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"

class GlslProgram
{
    
public:
    
    GlslProgram();
    void setupProgramFromFile(const std::string &vertShaderPath, const std::string &fragShaderPath);
    void setupProgramFromSource(const std::string &vertShaderSrc, const std::string &fragShaderSrc);
    void begin() const;
    void end() const;
    bool isLoaded() const;
    void setUniform1f(const std::string &uniformName, float v1) const;
    void setUniform2f(const std::string &uniformName, float v1, float v2) const;
    void setUniform3f(const std::string &uniformName, float v1, float v2, float v3) const;
    void setUniform4f(const std::string &uniformName, float v1, float v2, float v3, float v4) const;
    void setUniform4x4Matrix(const std::string &uniformName, const glm::mat4 &matrix) const;
    void setUniformSampler2D(const std::string &samplerName, GLint location) const;
    void setUniformSampler2D(const std::string &samplerName, const Image &img, GLint texUnit) const;
    
private:
    
    GLuint vertShaderID;
    GLuint fragShaderID;
    GLuint programID;
    bool bLoaded;
    static const int MAX_LOG_LENGTH = 4096;
    
    std::string loadFileToString(const std::string &filePath);
    void compileProgram(const std::string &vertShaderSrc, const std::string &fragShaderSrc);
    
};

#endif
