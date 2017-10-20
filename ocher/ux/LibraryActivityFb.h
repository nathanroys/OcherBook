/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_LIBRARYACTIVITY_H
#define OCHER_UX_FB_LIBRARYACTIVITY_H

#include "ocher/ux/Activity.h"
#include "ocher/ux/SystemBar.h"

#include <QRect>

class Meta;
class Settings;

class LibraryActivity : public Activity {
public:
    LibraryActivity();
    ~LibraryActivity();

    void draw();

    int evtKey(const struct OcherKeyEvent *);
    int evtMouse(const struct OcherMouseEvent *);

protected:
    void onAttached();
    void onDetached();

    SystemBar *m_systemBar;
    Settings *m_settings;
    const std::vector<Meta *> *m_library;
#define BOOKS_PER_PAGE 11
    QRect *m_bookRects;
    int itemHeight;
    unsigned int m_booksPerPage;
    unsigned int m_pages;
    unsigned int m_pageNum;
};

#endif
