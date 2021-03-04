//
// Created by wangrl on 2021/3/4.
//

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstdlib>

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main() {"
                                 "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}";

const char* fragmentShaderSourceOrange = "#version 330 core\n"
                                         "out vec4 fragColor;\n"
                                         "void main() {\n"
                                         "    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                         "}";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
                                         "out vec4 fragColor;\n"
                                         "void main() {"
                                         "    fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                         "}";

int main(int argc, char** argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "TwoTriangle", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, nullptr);
    glCompileShader(fragmentShaderOrange);

    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, nullptr);
    glCompileShader(fragmentShaderYellow);

    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    float orangeTriangle[] = {
            -0.9f, -0.5f, 0.0f,
            0.0f, -0.5f, 0.0f,
            -0.45f, 0.5f, 0.0f
    };

    float yellowTriangle[] = {
            0.0f, -0.5f, 0.0f,
            0.9f, -0.5f, 0.0f,
            0.45f, 0.5f, 0.0f
    };

    unsigned int vertexBufferObjects[2], vertexArrayObjects[2];
    glGenVertexArrays(2, vertexArrayObjects);
    glGenBuffers(2, vertexBufferObjects);

    // orange triangle setup
    glBindVertexArray(vertexArrayObjects[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(orangeTriangle), orangeTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // yellow triangle setup
    glBindVertexArray(vertexArrayObjects[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellowTriangle), yellowTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramOrange);
        glBindVertexArray(vertexArrayObjects[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramYellow);
        glBindVertexArray(vertexArrayObjects[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vertexArrayObjects);
    glDeleteVertexArrays(2, vertexBufferObjects);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);

    glfwTerminate();
    return EXIT_SUCCESS;
}
