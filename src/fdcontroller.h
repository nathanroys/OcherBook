/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FDCONTROLLER_H
#define OCHER_FDCONTROLLER_H

#include "controller.h"
#include "fdrender.h"

class FdController : public Controller {
public:
    FdController(Options* options);
    ~FdController();

    void run();

    FdRender m_renderer;
};

#endif
