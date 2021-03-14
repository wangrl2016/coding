//
// Created by wangrl on 2021/3/4.
//

#pragma once

#include "glad/glad.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm/mat4x4.hpp>

class Shader {
public:
    unsigned int id;

    Shader(const char* vertexPath, const char* fragmentPath, bool isShaderFile) {
        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        if (isShaderFile) {
            std::ifstream vertexShaderFile;
            std::ifstream fragmentShaderFile;

            // Ensure ifstream objects can throw exceptions.
            vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try {
                vertexShaderFile.open(vertexPath);
                fragmentShaderFile.open(fragmentPath);

                // Read file's buffer contents int streams.
                std::stringstream vertexShaderStream, fragmentShaderStream;
                vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf();
                vertexShaderFile.close();
                fragmentShaderFile.close();

                // Convert stream into string.
                vertexShaderSource = vertexShaderStream.str().c_str();
                fragmentShaderSource = vertexShaderStream.str().c_str();
            } catch (std::ifstream::failure& e) {
                std::cout << e.what() << std::endl;
            }
        } else {
            vertexShaderSource = vertexPath;
            fragmentShaderSource = fragmentPath;
        }

        // vertex shader
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // fragment shader
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");

        // shader program
        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use() {
        glUseProgram(id);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << infoLog << std::endl;
            }
        }
    }
};
