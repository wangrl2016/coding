//
// Created by wangrl on 2021/3/13.
//

#pragma once

#include <vector>

class Cuboid {
public:
    explicit Cuboid(float l = 0.5f, float w = 0.5f, float h = 0.5f);

    ~Cuboid() {}

    void set(float l, float w, float h);

    float getLength() const {
        return length;
    }

    float getWidth() const {
        return width;
    }

    float getHeight() const {
        return height;
    }

    const float* getVertices() const {
        return vertices.data();
    }

    const float* getNormals() const {
        return normals.data();
    }

    const float* getTexCoords() const {
        return texCoords.data();
    }

    const unsigned int* getIndices() const {
        return indices.data();
    }

    // 顶点的个数
    unsigned int getVertexCount() const {
        return (unsigned int) vertices.size() / 3;
    }

    unsigned int getNormalCount() const {
        return (unsigned int) normals.size() / 3;
    }

    unsigned int getTexCoordCount() const {
        return (unsigned int) texCoords.size() / 2;
    }

    unsigned int getIndexCount() const {
        return (unsigned int) indices.size();
    }

    unsigned int getVertexSize() const {
        return (unsigned int) vertices.size() * sizeof(float);
    }

    unsigned int getIndexSize() const {
        return (unsigned int) indices.size() * sizeof(float);
    }

private:

    void clearArrays();

    std::vector<float> vertices;        // 顶点坐标
    std::vector<float> normals;         // 法向量坐标
    std::vector<float> texCoords;       // 纹理坐标
    std::vector<unsigned int> indices;  // 顶点下标

    float length;   // 长方体的长度
    float width;    // 长方体的宽度
    float height;   // 长方体的高度
};
