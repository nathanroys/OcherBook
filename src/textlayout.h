/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FMT_TEXT_LAYOUT_H
#define OCHER_FMT_TEXT_LAYOUT_H

#include "layout.h"

class TextReader;

class LayoutText : public Layout {
public:
    LayoutText(const TextReader& text);
};

#endif
