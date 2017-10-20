/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CDKRENDER_H
#define OCHER_CDKRENDER_H

#include "render.h"

#include <cdk.h>
#include <vector>

class Pagination;

class CdkRender : public Render {
public:
    CdkRender(WINDOW* scr, CDKSCREEN* m_screen);

    int render(Pagination* pagination, unsigned int pageNum, bool doBlit);

    int outputWrapped(std::vector<char>* b, unsigned int strOffset, bool doBlit);

protected:
    WINDOW* m_scr;
    CDKSCREEN* m_screen;
    int m_width;
    int m_height;
    int m_x;
    int m_y;

    void enableUl();
    void disableUl();
    void enableEm();
    void disableEm();

    void pushAttrs();
    void applyAttrs(int i);
    void popAttrs();

    Attrs a[10];
    int ai;
};

#endif
