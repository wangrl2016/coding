//
// Created by wangrl on 2021/3/14.
//

#include "glad/glad.h"
#include "Cuboid.h"
#include "Shader.h"
#include "Utils.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm/gtc/matrix_transform.hpp>

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "out vec2 texCoord;\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main() {\n"
                                 "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                 "}";

const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 fragColor;\n"
                                   "void main() {\n"
                                   "    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main(int argc, char** argv) {
    GLFWwindow* window = createOpenGLWindow(600, 600, "CameraView");
    if (window == nullptr)
        exit(EXIT_FAILURE);

    glEnable(GL_DEPTH_TEST);

    Shader shader(vertexShaderSource, fragmentShaderSource, false);
    Cuboid sphere(0.6, 0.6, 0.6);

    unsigned int vertexArrayObject, vertexBufferObject /* , elementBufferObject */;
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    // Bind the vertex array object first,
    // then bind and set vertex buffer,
    // and then configure vertex attributes.
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexSize(), sphere.getVertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, sphere.getIndices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);

    glfwTerminate();
    return EXIT_SUCCESS;
}
