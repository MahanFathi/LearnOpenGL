#pragma once

#include <iostream>
#include <math.h>

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <streambuf>

class Shader {

public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();

    void setUniform(const std::string &name, float value) const;
    void setUniform(const std::string &name, int value) const;
    void setUniform(const std::string &name, bool value) const;

};
