/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FORMAT_H
#define OCHER_FORMAT_H

#include <string>
#include <vector>

enum Encoding {
    OCHER_ENC_UNKNOWN,
    OCHER_ENC_UTF8,
    OCHER_ENC_UTF16BE,
    OCHER_ENC_UTF16LE,
    OCHER_ENC_UTF32BE,
    OCHER_ENC_UTF32LE,
};

enum Fmt {
    OCHER_FMT_UNKNOWN,
    OCHER_FMT_EPUB,
    OCHER_FMT_TEXT,
    OCHER_FMT_HTML,
};

const char* fmtToStr(Fmt fmt);
bool stripUtf8Bom(std::string& data);
Fmt detectFormat(const char* file, Encoding* encoding);

class Reader {
public:
    virtual std::unique_ptr<std::vector<char>> layout() = 0;
};

#include "epub.h"
#include "epublayout.h"
#include "text.h"
#include "textlayout.h"

#endif
