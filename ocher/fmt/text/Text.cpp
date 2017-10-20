/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/fmt/text/Text.h"
#include "ocher/util/File.h"


Text::Text(const std::string &filename)
{
    File f(filename);

    f.readRest(m_text);
}
