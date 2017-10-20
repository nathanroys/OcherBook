/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_EPUBLAYOUT_H
#define OCHER_EPUBLAYOUT_H

#include "layout.h"
#include "logger.h"

#include "mxml.h"

class EpubReader;

class EpubLayout : public Layout {
public:
    EpubLayout(EpubReader* epub)
        : m_epub(epub)
        , m_log(Log::get("EpubLayout"))
    {
    }

#if 1
    void append(mxml_node_t* tree);
#else
    void append(std::string& html);
#endif

protected:
    void processNode(mxml_node_t* node);
    void processSiblings(mxml_node_t* node);

    EpubReader* m_epub;
    Logger* m_log;
};

#endif
