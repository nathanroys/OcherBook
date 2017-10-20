/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "fdread.h"

#include "ebook.h"
#include "fdcontroller.h"
#include "render.h"

#include <assert.h>

FdRead::FdRead(FdController* c, EBook* ebook)
    : m_log(Log::get("FdRead"))
    , m_controller(c)
    , m_ebook(ebook)
    , m_pageNum(0)
    , atEnd(1)
{
    m_ebook->record.touch();
    m_pageNum = m_ebook->record.activePage;
}

void FdRead::draw()
{
    atEnd = m_controller->m_renderer.render(&m_ebook->m_pagination, m_pageNum, true);
}
