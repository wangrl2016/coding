//
// Created by wangrl on 2021/3/14.
//

#include <cstdlib>
#include "Utils.h"
#include "Shader.h"

int main(int argc, char** argv) {
    GLFWwindow* window = createOpenGLWindow(600, 600, "Specular");
    if (window == nullptr)
        return EXIT_FAILURE;

    glEnable(GL_DEPTH_TEST);

}