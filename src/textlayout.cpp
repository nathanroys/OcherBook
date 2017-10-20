/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "textlayout.h"
#include "text.h"

LayoutText::LayoutText(const TextReader& text)
{
    int sawNl = 0;
    unsigned int n = text.m_text.size();
    const char* raw = text.m_text.data();

    for (unsigned int i = 0; i < n; ++i) {
        // TODO: heuristics to detect centered lines

        // \n\n means real line break; otherwise reflow text
        if (raw[i] == '\n') {
            if (sawNl) {
                outputNl();
                outputBr();
                continue;
            } else
                sawNl = 1;
        } else if (raw[i] == '\f') {
            outputPageBreak();
            continue;
        } else {
            sawNl = 0;
        }
        outputChar(raw[i]);
    }
    flushText();
}
