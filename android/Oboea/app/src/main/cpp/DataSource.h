//
// Created by wangrl on 21-4-14.
//

#pragma once

#include <cstdint>

#include "GameConstants.h"

class DataSource {
public:
    virtual ~DataSource() {}

    virtual int64_t getSize() const = 0;

    virtual AudioProperties getProperties() const = 0;

    virtual const float *getData() const = 0;
};
