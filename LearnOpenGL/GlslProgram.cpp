#include "glslProgram.h"

// ===============================
// Public member functions
// ===============================

GlslProgram::GlslProgram()
{
    
}

void GlslProgram::setupProgramFromFile(const std::string &vertShaderPath, const std::string &fragShaderPath)
{
    std::string vertShaderSource = loadFileToString(vertShaderPath);
    std::string fragShaderSource = loadFileToString(fragShaderPath);
    compileProgram(vertShaderSource, fragShaderSource);
}

void GlslProgram::setupProgramFromSource(const std::string &vertShaderSrc, const std::string &fragShaderSrc)
{
    compileProgram(vertShaderSrc, fragShaderSrc);
}

bool GlslProgram::isLoaded() const
{
    return bLoaded;
}

void GlslProgram::begin() const
{
    glUseProgram(programID);
}

void GlslProgram::end() const
{
    glUseProgram(0);
}

/* 
 * Note that finding the uniform location does not require you to use the shader program first, but
 * updating a uniform does require you to first use the program (by calling glUseProgram), because it 
 * sets the uniform on the currently active shader program. So, these calls will only work between
 * glslProgram::begin and glslProgram end.
 */
void GlslProgram::setUniform1f(const std::string &uniformName, float v1) const
{
    GLint uniformLocation = glGetUniformLocation(programID, uniformName.c_str());
    if (uniformLocation != -1) glUniform1f(uniformLocation, v1);
}

void GlslProgram::setUniform2f(const std::string &uniformName, float v1, float v2) const
{
    GLint uniformLocation = glGetUniformLocation(programID, uniformName.c_str());
    if (uniformLocation != -1) glUniform2f(uniformLocation, v1, v2);
}

void GlslProgram::setUniform3f(const std::string &uniformName, float v1, float v2, float v3) const
{
    GLint uniformLocation = glGetUniformLocation(programID, uniformName.c_str());
    if (uniformLocation != -1) glUniform3f(uniformLocation, v1, v2, v3);
}

void GlslProgram::setUniform4f(const std::string &uniformName, float v1, float v2, float v3, float v4) const
{
    GLint uniformLocation = glGetUniformLocation(programID, uniformName.c_str());
    if (uniformLocation != -1) glUniform4f(uniformLocation, v1, v2, v3, v4);
}

void GlslProgram::setUniform4x4Matrix(const std::string &uniformName, const glm::mat4 &matrix) const
{
    /*
     * The parameters of glUniformMatrix4fv are as follows:
     * 1) The uniform location
     * 2) The # of matrices we want to send
     * 3) Should the matrix be transposed?
     * 4) The actual matrix data, transformed into an OpenGL-ready format
     */
    GLuint uniformLoc = glGetUniformLocation(programID, uniformName.c_str());
    if (uniformLoc != -1) glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void GlslProgram::setUniformSampler2D(const std::string &samplerName, GLint location) const
{
    /* 
     * Note that we're using glUniform1i to set the location or texture unit of the uniform samplers.
     * By setting them via glUniform1i we make sure each uniform sampler corresponds to the proper texture unit.
     */
    glActiveTexture(GL_TEXTURE0 + location);
    GLint uniformLocation = glGetUniformLocation(programID, samplerName.c_str());
    if (uniformLocation != -1) glUniform1i(uniformLocation, location);
}

// ===============================
// Private member functions
// ===============================

std::string GlslProgram::loadFileToString(const std::string &filePath)
{
    std::string fileData;
    std::fstream stream(filePath);
    if (stream.is_open())
    {
        std::string line = "";
        while (std::getline(stream, line))
            fileData += "\n" + line;
        stream.close();
    }
    else
    {
        std::cerr << "Failed to open file stream." << std::endl;
    }
    return fileData;
}

void GlslProgram::compileProgram(const std::string &vertShaderSrc, const std::string &fragShaderSrc)
{
    vertShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // glShaderSource requires C-style string parameters, so we do the conversion here
    const char *rawVertShaderSource = vertShaderSrc.c_str();
    const char *rawFragShaderSource = fragShaderSrc.c_str();
    
    glShaderSource(vertShaderID, 1, &rawVertShaderSource, NULL);
    glShaderSource(fragShaderID, 1, &rawFragShaderSource, NULL);
    
    GLint success = 0;
    
    // Vertex shader
    glCompileShader(vertShaderID);
    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
    
    // Error logging
    if (!success)
    {
        std::cerr << "ERROR: compiling vertex shader.\n";
        char buffer[MAX_LOG_LENGTH];
        glGetShaderInfoLog(vertShaderID, MAX_LOG_LENGTH, NULL, buffer);
        
        for (int i = 0; i < MAX_LOG_LENGTH; ++i)
            std::cerr << buffer[i];
        glDeleteShader(vertShaderID);
        bLoaded = false;
        return;
    }
    
    success = 0;
    
    // Fragment shader
    glCompileShader(fragShaderID);
    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
    
    // Error logging
    if (!success)
    {
        std::cerr << "ERROR: compiling fragment shader.\n";
        char buffer[MAX_LOG_LENGTH];
        glGetShaderInfoLog(fragShaderID, MAX_LOG_LENGTH, NULL, buffer);
        
        for (int i = 0; i < MAX_LOG_LENGTH; ++i)
            std::cerr << buffer[i];
        glDeleteShader(fragShaderID);
        bLoaded = false;
        return;
    }
    
    programID = glCreateProgram();
    glAttachShader(programID, vertShaderID);
    glAttachShader(programID, fragShaderID);
    
    success = 0;
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        std::cerr << "ERROR: linking program object.\n";
        char buffer[MAX_LOG_LENGTH];
        glGetProgramInfoLog(programID, MAX_LOG_LENGTH, NULL, buffer);
        
        for (int i = 0; i < MAX_LOG_LENGTH; ++i)
            std::cerr << buffer[i];
        bLoaded = false;
        return;
    }
    
    // Once we've linked our shaders into a program object, we don't need them anymore
    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);
    bLoaded = true;
    
    std::cout << "Successfully loaded shader sources." << std::endl;
}