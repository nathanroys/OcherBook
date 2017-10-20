/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "pagination.h"

#include <assert.h>

Pagination::Pagination()
    : m_numPages(0)
{
}

Pagination::~Pagination()
{
    flush();
}

void Pagination::flush()
{
    m_numPages = 0;
    for (auto it = m_pages.begin(); it < m_pages.end(); ++it) {
        delete *it;
    }
    m_pages.clear();
}

void Pagination::set(
    unsigned int pageNum, unsigned int layoutOffset, unsigned int strOffset /* TODO attrs */)
{
    assert(pageNum <= m_numPages);
    unsigned int chunk = pageNum / pagesPerChunk;
    if (pageNum == m_numPages) {
        if (chunk == m_pages.size()) {
            m_pages.push_back(new PageMapping[pagesPerChunk]);
        }
    }
    struct PageMapping* mapping = (struct PageMapping*)m_pages[chunk];
    mapping += pageNum % pagesPerChunk;
    if (mapping->layoutOffset != layoutOffset || mapping->strOffset != strOffset) {
        mapping->layoutOffset = layoutOffset;
        mapping->strOffset = strOffset;
        if (m_numPages <= pageNum)
            m_numPages = pageNum + 1;
    }
}

bool Pagination::get(
    unsigned int pageNum, unsigned int* layoutOffset, unsigned int* strOffset /* TODO attrs */)
{
    if (pageNum > m_numPages)
        return false;
    unsigned int chunk = pageNum / pagesPerChunk;
    if (chunk > m_pages.size()) {
        return false;
    }
    struct PageMapping* mapping = (struct PageMapping*)m_pages[chunk];
    mapping += pageNum % pagesPerChunk;
    *layoutOffset = mapping->layoutOffset;
    *strOffset = mapping->strOffset;
    return true;
}
