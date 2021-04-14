//
// Created by wangrl on 21-4-14.
//

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

struct ScreenColor {
    float red;
    float green;
    float blue;
    float alpha;
};

constexpr ScreenColor RED{1.0f, 0.0f, 0.0f, 1.0f};
constexpr ScreenColor GREEN{0.0f, 1.0f, 0.0f, 1.0f};
constexpr ScreenColor BLUE{0.0f, 0.0f, 1.0f, 1.0f};
constexpr ScreenColor PURPLE{1.0f, 0.0f, 1.0f, 1.0f};
constexpr ScreenColor ORANGE{1.0f, 0.5f, 0.0f, 1.0f};
constexpr ScreenColor GREY{0.3f, 0.3f, 0.3f, 0.3f};
constexpr ScreenColor YELLOW{1.0f, 1.0f, 0.0f, 1.0f};

void CheckOpenGLError(const char *stmt, const char *name, int line);

void SetGLScreenColor(ScreenColor color);
