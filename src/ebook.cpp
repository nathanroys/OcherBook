/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ebook.h"

#include <time.h>

void BookRecord::touch()
{
    touched = time(NULL);
}

EBook::EBook()
    : format(OCHER_FMT_UNKNOWN)
    , encoding(OCHER_ENC_UNKNOWN)
    , pages(0)
{
}

unsigned int EBook::percentRead()
{
    if (pages == 0)
        return 0;
    if (record.activePage > pages)
        return 100;
    return record.activePage * 100 / pages;
}

void EBook::layout()
{
    // TODO OCHER-1
    // This is massively broken:
    // - Layout::unlock returns a buffer
    // - ~Layout deletes strings referenced by this buffer
    // --> Encapsulate all this within on object.
    //     The Layerouter-thing gives you a Layout.
    // --> The Render gets a Layout (probably not a full EBook) not a raw vector<char>.
    //
    // Just leaking for now, in FooReader::layout().
    const char* file = relPath.c_str();
    switch (format) {
    case OCHER_FMT_TEXT: {
        TextReader text(file);
        m_layout = text.layout();
        break;
    }
    case OCHER_FMT_EPUB: {
        EpubReader epub(file);
        m_layout = epub.layout();
        break;
    }
    default: {
        // m_log->warn("Unhandled format %d", format);
    }
    }
}
