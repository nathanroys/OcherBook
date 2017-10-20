/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FDRENDER_H
#define OCHER_FDRENDER_H

#include "render.h"

#include <vector>

class FdRender : public Render {
public:
    FdRender(int fd);

    int render(Pagination* pagination, unsigned int pageNum, bool doBlit);

    int outputWrapped(std::vector<char>* b, unsigned int strOffset, bool doBlit);

protected:
    int m_isTty;

    int m_fd;
    int m_width;
    int m_height;
    int m_x;
    int m_y;

    void clearScreen();
    void enableUl();
    void disableUl();
    void enableEm();
    void disableEm();

    // XXX split state

    void pushAttrs();
    void applyAttrs(int i);
    void popAttrs();

    Attrs a[10];
    int ai;
};

#endif
