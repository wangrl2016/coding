//
// Created by wangrl on 2021/3/10.
//

#pragma once

#include <vector>

/**
 * flat模式三角形三个顶点的法向量一致，也就是三角形的法向量。
 * smooth模式为每个顶点创建一个法向量，看起来更光滑。
 */

class Sphere {
public:
    Sphere(float radius = 1.0f, int sectorCount = 46,
           int stackCount = 18, bool smooth = true);

    ~Sphere();

    float getRadius() const {
        return radius;
    }

    float getSectorCount() const {
        return sectorCount;
    };

    float getStackCount() const {
        return stackCount;
    }

    void set(float radius, int sectorCount, int stackCount, bool smooth = true);

    void setRadius(float radius);

    void setSectorCount(int sectorCount);

    void setStackCount(int stackCount);

    void setSmooth(bool smooth);

private:
    float radius;           // 球的半径
    int sectorCount;        // 经度划分的个数
    int stackCount;         // 纬度划分的个数

    bool smooth;
    std::vector<float> vertices;    // 顶点坐标
    std::vector<float> normals;     // 法向量坐标
    std::vector<float> texCoords;   // 纹理坐标

};
