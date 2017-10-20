/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CDKCONTROLLER_H
#define OCHER_CDKCONTROLLER_H

#include "cdkrender.h"
#include "controller.h"

#include <cdk.h>

class CdkController : public Controller {
public:
    CdkController(Options* options);
    ~CdkController();

    void run();

    WINDOW* m_scr;
    CDKSCREEN* m_screen;

    CdkRender* m_renderer;
};

#endif
