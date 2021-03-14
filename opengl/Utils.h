//
// Created by wangrl on 2021/3/14.
//

#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

/**
 * 初始化OpenGL接口，并且创建GLFW窗口。
 *
 * @param width     窗口的宽度
 * @param height    窗口的高度
 * @param filename  窗口的名字
 * @return 成功返回GLFWWindow窗口指针
 */
GLFWwindow* createOpenGLWindow(int width, int height, const char* filename);

/**
 * 加载纹理。
 *
 * @param filename 纹理的路径
 * @return 纹理句柄
 */
unsigned int loadTexture(const char* filename);