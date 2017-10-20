/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "cdkcontroller.h"

#include "cdkread.h"
#include "cdkrender.h"
#include "options.h"
#include "syncbooks.h"

#include <assert.h>

CdkController::CdkController(Options* options)
    : Controller(options)
    , m_scr(0)
    , m_screen(0)
    , m_renderer(0)
{
    m_scr = initscr();
    m_screen = initCDKScreen(m_scr);
    initCDKColor();

    m_renderer = new CdkRender(m_scr, m_screen);
}

CdkController::~CdkController()
{
    delete m_renderer;
    m_renderer = 0;

    destroyCDKScreen(m_screen);
    endCDK();
    delwin(m_scr);
}

void CdkController::run()
{
    // TODO:  This is all hard-coded.

    SyncBooks(this, m_options->files);

    auto metas = ctx.library.getList();
    if (metas->size() > 0) {
        EBook* selected = (*metas)[0];
        selected->layout();

        // TODO Pass a Layout object, not a raw buffer pointer
        m_renderer->set(*selected->m_layout.get());

        // CdkRead
    }
}
