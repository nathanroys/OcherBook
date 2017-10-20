/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FDREAD_H
#define OCHER_FDREAD_H

#include "ebook.h"
#include "fdactivity.h"
#include "logger.h"

class FdController;
class Renderer;

class FdRead : public FdActivity {
public:
    FdRead(FdController* c, EBook* ebook);

    // protected:
    void next() { m_pageNum++; } // XXX

    void draw();

protected:
    Logger* m_log;
    FdController* m_controller;
    EBook* m_ebook;

    unsigned int m_pageNum;
    int atEnd;
};

#endif
