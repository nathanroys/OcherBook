/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/shelf/Meta.h"
#include "ocher/ux/cdk/BrowseActivityCdk.h"
#include "ocher/util/Logger.h"

#define LOG_NAME "ocher.ux.Library"


BrowseActivityCdk::BrowseActivityCdk(UxControllerCdk *uxController) :
    ActivityCdk(uxController)
{
}

void BrowseActivityCdk::onAttached()
{
    Log::info(LOG_NAME, "attached");

}

void BrowseActivityCdk::onDetached()
{
    Log::info(LOG_NAME, "detached");
}

Meta *BrowseActivityCdk::browse(std::vector<Meta *> &meta)
{
    const unsigned int nItems = meta.size();
    char *items[nItems];

    for (unsigned int i = 0; i < nItems; ++i) {
        Meta *m = meta[i];
        items[i] = m->relPath.c_str();
    }

    CDKSCROLL *scroll = newCDKScroll(m_screen, LEFT, TOP, RIGHT, 0, 0, "Select a book...",
            items, nItems, 0, 0, 1, 1);
    int r = activateCDKScroll(scroll, NULL);
    destroyCDKScroll(scroll);
    if (r == -1) {
        return (Meta *)0;
    } else {
        return meta[r];
    }
}

void BrowseActivityCdk::read(Meta *meta)
{
    Renderer *renderer = uiFactory->getRenderer();

    for (int pageNum = 0;; ) {
        int atEnd = renderer->render(&meta->m_pagination, pageNum, true);

        int key = getch();
        if (key == 'p' || key == 'b') {
            if (pageNum > 0)
                pageNum--;
        } else if (key == 'q' || atEnd) {
            break;
        } else {
            pageNum++;
        }
    }
}
