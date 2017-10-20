/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_RENDERER_H
#define OCHER_UX_RENDERER_H

#include <stdint.h>
#include <vector>

class Pagination;

/**
 */
class Attrs {
public:
    Attrs()
        : pre(0)
        , ws(0)
        , nl(0)
        , ul(0)
        , b(0)
        , em(0)
        , pad(0)
        , pts(12)
    {
    }
    int pre : 1;
    int ws : 1;
    int nl : 1;
    int ul : 1; ///< underline
    int b : 1; ///< bold
    int em : 1; ///< emphasize/italics
    int pad : 2;
    uint8_t pts; ///< text points
} __attribute__((packed));

/**
 */
class Render {
public:
    virtual ~Render() {}

    virtual bool init() { return true; }

    virtual void set(const std::vector<char>& layout) { m_layout = layout; }

    /**
     * Render the page.
     * @return -1 if this is an unknown page (error; prior page not paginated),
     *  0 if reached the end of the page and it overflowed;
     *  1 if reached the end of the layout (no overflow)
     */
    virtual int render(Pagination* pagination, unsigned int pageNum, bool doBlit) = 0;

protected:
    std::vector<char> m_layout;
};

#endif
