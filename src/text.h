/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FMT_TEXT_H
#define OCHER_FMT_TEXT_H

#include "format.h"

/** Plain-text file format reader
 */
class TextReader : public Reader {
public:
    TextReader(const std::string& filename);

    virtual ~TextReader() {}

    std::unique_ptr<std::vector<char>> layout();

    std::string m_text;
};

#endif
