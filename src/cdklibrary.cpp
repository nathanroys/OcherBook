/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "cdklibrary.h"

#include "cdkcontroller.h"
#include "cdkrender.h"
#include "ebook.h"

#include <cdk.h>

CdkLibrary::CdkLibrary(CdkController* controller)
    : m_controller(controller)
{
}

void CdkLibrary::onAttached()
{
}

void CdkLibrary::onDetached()
{
}

EBook* CdkLibrary::browse(std::vector<EBook*>& ebooks)
{
    const unsigned int nItems = ebooks.size();
    const char* items[nItems];

    for (unsigned int i = 0; i < nItems; ++i) {
        EBook* m = ebooks[i];
        items[i] = m->relPath.c_str();
    }

    // TODO:  Is this really char **?
    CDKSCROLL* scroll = newCDKScroll(m_controller->m_screen, LEFT, TOP, RIGHT, 0, 0,
        "Select a book...", (char**)items, nItems, 0, 0, 1, 1);
    int r = activateCDKScroll(scroll, NULL);
    destroyCDKScroll(scroll);
    if (r == -1) {
        return nullptr;
    } else {
        return ebooks[r];
    }
}

void CdkLibrary::read(EBook* ebook)
{
    for (int pageNum = 0;;) {
        int atEnd = m_controller->m_renderer->render(&ebook->m_pagination, pageNum, true);

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
