/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_PAGINATION_H
#define OCHER_PAGINATION_H

#include <vector>

/**
 * Stores a mapping from a page number to offsets within the Layout.
 *
 * @todo  persist pagination for faster random access (per everything that might affect layout:
 * ocher version, book modification time, layout prefs, ...)
 */
class Pagination {
public:
    Pagination();
    ~Pagination();

    /**
     * Must flush when any layout parameters change.
     */
    void flush();

    /**
     * Sets a mapping from a page to offsets within the Layout.  Setting a page invalidates all
     * subsequent pages.
     */
    void set(unsigned int page, unsigned int layoutOffset, unsigned int strOffset /* TODO attrs */);

    bool get(unsigned int page, unsigned int* layoutOffset, unsigned int* strOffset /* TODO attrs */);

    unsigned int numPages() const
    {
        return m_numPages;
    }

protected:
    struct PageMapping {
        /** @todo to handle large epubs on small machines, may need to break up
         * the layout per spine index */
        // unsigned int spineIndex;
        unsigned int layoutOffset;    /* TODO xxx  this is uninitialized */
        unsigned int strOffset;
        /* TODO attrs */
    };
    static const unsigned int pagesPerChunk = 100;
    std::vector<PageMapping*> m_pages;
    unsigned int m_numPages;
};


#endif
