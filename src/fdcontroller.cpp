/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "fdcontroller.h"

#include "fdread.h"
#include "fdrender.h"
#include "options.h"
#include "syncbooks.h"

FdController::FdController(Options* options)
    : Controller(options)
    , m_renderer(options->inFd)
{
}

FdController::~FdController()
{
}

void FdController::run()
{
    // TODO:  This is all hard-coded.

    SyncBooks(this, m_options->files);

    auto metas = ctx.library.getList();
    if (metas->size() > 0) {
        EBook* selected = (*metas)[0];
        selected->layout();

        // TODO Pass a Layout object, not a raw buffer pointer
        m_renderer.set(*selected->m_layout.get());

#if 0
// Optionally, run through all pages without blitting to get an accurate
// page count.  Alternative is to do some sort of "idealize" page layout that might be faster.
    if (m_pagination.numPages() == 0) {
        for (int pageNum = 0;; pageNum++) {
            //m_log->info("Paginating page %d", pageNum);
            int r = m_controller->m_renderer.render(&m_pagination, pageNum, false);

            if (r != 0) {
                pages = pageNum + 1;
                break;
            }
        }
    }
#endif

        FdRead read(this, selected);
        for (int page = 0; page < 10; ++page) {
            printf("--- page %d of %d ---\n", page, selected->pages);
            read.draw();
            read.next();
        }
    }
}
