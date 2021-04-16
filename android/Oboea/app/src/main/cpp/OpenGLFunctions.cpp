//
// Created by wangrl on 21-4-14.
//

#include <cassert>
#include "OpenGLFunctions.h"
#include "LogMacros.h"

void CheckOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        LOGW("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        assert(false);
    }
}


void SetGLScreenColor(ScreenColor color) {
    glClearColor(color.red, color.green, color.blue, color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

