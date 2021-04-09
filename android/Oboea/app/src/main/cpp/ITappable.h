//
// Created by wangrl on 21-4-9.
//

#pragma once

class ITappable {
public:
    virtual ~ITappable() = default;

    virtual void tap(bool isDown) = 0;
};
