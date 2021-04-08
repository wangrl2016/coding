//
// Created by wangrl on 2021/4/6.
//

#pragma once

namespace sf {
    /**
     * Base class for classes that require an OpenAL context.
     *
     * This class is for internal use only, it must be the base
     * of every class that requires a valid OpenAL context in
     * order to work.
     */
    class AlResource {
    protected:
        AlResource();

        ~AlResource();
    };
}
