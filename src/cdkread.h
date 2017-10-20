/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CDKREAD_H
#define OCHER_CDKREAD_H

#include "cdkactivity.h"
#include "ebook.h"
#include "logger.h"

class CdkController;
class Renderer;
class Layout;

class CdkRead : public CdkActivity {
public:
    CdkRead(CdkController* c);

protected:
    void draw();
    void onAttached();
    void onDetached();

    Logger* m_log;
    CdkController* m_controller;
    EBook* ebook;
    unsigned int m_pageNum;
    int atEnd;
};

#endif
