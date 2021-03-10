//
// Created by wangrl on 2021/3/10.
//

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


}
