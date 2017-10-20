/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "text.h"

#include <fstream>

TextReader::TextReader(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
    if (ifs.good()) {
        m_text
            = std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    }
    // TODO else...?
}

std::unique_ptr<std::vector<char>> TextReader::layout()
{
    // TODO OCHER-1
    LayoutText* layout = new LayoutText(*this);
    return layout->unlock();
}
