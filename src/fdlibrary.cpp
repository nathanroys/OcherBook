/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "fdlibrary.h"
#include "ebook.h"
#include "fdcontroller.h"

#include <stdio.h>
#include <unistd.h>

FdLibrary::FdLibrary(FdController* c)
    : m_controller(c)
    , m_library(0)
    , m_pageNum(0)
{
}

FdLibrary::~FdLibrary()
{
}

void FdLibrary::draw()
{
    printf("\n");
    for (unsigned int i = 0; i < m_library->size(); ++i) {
        EBook* m = (*m_library)[i];
        // TODO:  title/author
        // TODO:  pad to width
        printf("%3u: %s\n", i + 1, m->title.c_str());
        printf("     %s\n", m->author.c_str());
        printf("     %4s  %d of %d\n", fmtToStr(m->format), 0, 100);
    }

    // char key = getKey();
    // return (EBook *)m_library->get(key - '1');
}

void FdLibrary::onAttached()
{
    m_library = m_controller->ctx.library.getList();
    m_pages = (m_library->size() + m_booksPerPage - 1) / m_booksPerPage;
}

#if 0
void BrowseFd::read(EBook *ebook)
{
    Renderer *renderer = uiFactory->getRenderer();

    for (int pageNum = 0;; ) {
        int atEnd = renderer->render(&ebook->m_pagination, pageNum, true);

        char buf[16];
        sprintf(buf, "%u of %u: ", pageNum + 1, ebook->m_pagination.numPages() + 1);
        write(m_out, buf, strlen(buf));

        char key = getKey();
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
#endif
