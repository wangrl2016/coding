//
// Created by wangrl on 2021/3/13.
//

#include "Cuboid.h"
#include <glm/glm.hpp>

Cuboid::Cuboid(float l, float w, float h) {
    set(l, w, h);
}


void Cuboid::set(float l, float w, float h) {
    this->length = l;
    this->width = w;
    this->height = h;

    clearArrays();

    /**
     * 计算长方体八个顶点坐标
     *          y
     *          ^
     *          |
     *      4--------5
     *     /|       /|
     *    / |      / |
     *    0-|-----1  |   -> x
     *    | 6-----|--7
     *    |/      | /
     *    2-------3
     *       /
     *      z
     */

    std::vector<glm::vec3> vv;

    vv.emplace_back(-l / 2, w / 2, h / 2);
    vv.emplace_back(l / 2, w / 2, h / 2);
    vv.emplace_back(-l / 2, -w / 2, h / 2);
    vv.emplace_back(l / 2, -w / 2, h / 2);

    vv.emplace_back(-l / 2, w / 2, -h / 2);
    vv.emplace_back(l / 2, w / 2, -h / 2);
    vv.emplace_back(-l / 2, -w / 2, -h / 2);
    vv.emplace_back(l / 2, -w / 2, -h / 2);

    for (int i = 0; i < vv.size(); i++) {
        vertices.push_back(vv[i][0]);
        vertices.push_back(vv[i][1]);
        vertices.push_back(vv[i][2]);
    }


    indices = {
            0, 1, 2,  // 1
            1, 3, 2,  // 2
            1, 5, 3,  // 3
            5, 7, 3,  // 4
            4, 5, 6,  // 5
            5, 7, 6,  // 6
            4, 0, 2,  // 7
            0, 2, 6,  // 8
            0, 4, 5,  // 9
            5, 1, 0,  // 10
            2, 6, 7,  // 11
            7, 3, 2   // 12
    };
}

void Cuboid::clearArrays() {
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
}
