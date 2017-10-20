/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_EBOOK_H
#define OCHER_EBOOK_H

#include "format.h"
#include "pagination.h"

#include <memory>
#include <string>
#include <sys/time.h>

/** User-specific and book-specific metadata.
 */
class BookRecord {
public:
    BookRecord()
        : activePage(0)
        , touched(0)
        , shortlist(0)
    {
    }

    unsigned int activePage; // TODO:  can't use page number; varies by device/settings

    void touch();
    time_t touched;

    int shortlist;

    // TODO: tags

    // TODO: dates
    // TODO: highlights
    // TODO: annotations
    // TODO: notes
};

/** Metadata associated with each book.
 *
 * This is the root object for each e-book such as the format, the layout, metadata, user's reading
 * record, etc.  Note the separation between book-specific and user-specific data.
 *
 * TODO: refcounted by views (Shelf)?
 */
class EBook {
public:
    EBook();

    void layout();

    Fmt format;
    Encoding encoding;
    std::string relPath; // TODO: for now, full path

    std::string author;
    std::string title;
    std::string language;
    std::string icon;

    BookRecord record;

    unsigned int percentRead();

    Pagination m_pagination;
    unsigned int pages; // is this even meaningful?
    std::unique_ptr<std::vector<char>> m_layout;
};

#endif
