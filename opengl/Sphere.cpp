//
// Created by wangrl on 2021/3/10.
//

#include <math.h>
#include "Sphere.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

Sphere::Sphere(float radius, int sectors, int stacks, bool smooth) :
        interleavedStride(32) {
    set(radius, sectors, stacks, smooth);
}

void Sphere::set(float radius, int sectors, int stacks, bool smooth) {
    this->radius = radius;
    this->sectorCount = (sectors < MIN_SECTOR_COUNT) ? MIN_SECTOR_COUNT : sectors;
    this->stackCount = (stacks < MIN_STACK_COUNT) ? MIN_STACK_COUNT : stacks;
    this->smooth = smooth;

    if (smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();

}

void Sphere::setRadius(float radius) {
    if (radius != this->radius)
        set(radius, sectorCount, stackCount, smooth);
}

void Sphere::setSectorCount(int sectors) {
    if (sectors != this->sectorCount)
        set(radius, sectors, stackCount, smooth);
}

void Sphere::setStackCount(int stacks) {
    if (stacks != this->stackCount)
        set(radius, sectorCount, stacks, smooth);
}

void Sphere::setSmooth(bool smooth) {
    if (smooth != this->smooth)
        set(radius, sectorCount, stackCount, smooth);
}


void Sphere::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

/**
 * Build vertices of sphere with smooth shading using parametric equation.
 *
 * x = r * cos(u) * cos(v)
 * y = r * cos(u) * sin(v)
 * z = r * sin(u)
 *
 * where u: stack(latitude) angle (-90 <= u <= 90)
 *       v: sector(longitude) angle (0 <= v <= 360)
 */
void Sphere::buildVerticesSmooth() {

    // Clear memory of prev arrays.
    clearArrays();

    float x, y, z, xy;      // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;             // texCoord

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;  // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle); // r * cos(u)
        z = radius * sinf(stackAngle);  // r * sin(u)

        // Add (sectorCount+1) vertices per stack.
        // The first and last vertices have same position and normal, but
        // different texCoords.
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            // vertex position
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            addVertex(x, y, z);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            addNormal(nx, ny, nz);

            // vertex tex coord between [0, 1]
            s = (float) j / sectorCount;
            t = (float) i / stackCount;
            addTexCoord(s, t);
        }
    }

    // indices
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1); // beginning of current stack
        k2 = (i + 1) * (sectorCount + 1);  // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0)
                addIndices(k1, k2, k1 + 1);   // k1--k2--k1+1
            if (i != (stackCount - 1))
                addIndices(k1 + 1, k2, k2 + 1); // k1+1--k2--k2+1

            // Vertical lines for all stacks.
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0) {
                // Horizontal lines except 1st stack
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }
}

void Sphere::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Sphere::addNormal(float nx, float ny, float nz) {
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}

void Sphere::addTexCoord(float s, float t) {
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void Sphere::addIndices(int k1, int k2, int k3) {
    indices.push_back(k1);
    indices.push_back(k2);
    indices.push_back(k3);
}

void Sphere::buildVerticesFlat() {

}
