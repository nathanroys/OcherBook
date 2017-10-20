/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CONTROLLER_H
#define OCHER_CONTROLLER_H

#include "shelf.h"

class Options;

class ReadingContext {
public:
    ReadingContext()
        : shortList(&library)
    {
    }

    Library library;
    ShortList shortList;
};

class Controller {
public:
    Controller(Options* options);

    virtual ~Controller();

    virtual void run() = 0;

    ReadingContext ctx;

protected:
    Options* m_options;

    void initLog();
};

#endif
