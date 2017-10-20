/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FDLIBRARY_H
#define OCHER_FDLIBRARY_H

#include "fdactivity.h"

#include <vector>

class FdController;
class EBook;

class FdLibrary : public FdActivity {
public:
    FdLibrary(FdController* c);
    ~FdLibrary();

    void draw();

protected:
    void onAttached();

    FdController* m_controller;

    const std::vector<EBook*>* m_library;
#define BOOKS_PER_PAGE 11
    int itemHeight;
    unsigned int m_booksPerPage;
    unsigned int m_pages;
    unsigned int m_pageNum;
};

#endif
