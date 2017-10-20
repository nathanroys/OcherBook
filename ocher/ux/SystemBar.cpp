/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/settings/Settings.h"
#include "ocher/ux/SystemBar.h"


SystemBar::SystemBar(Battery *battery) :
    m_sep(false),
    m_batteryIcon(battery),
    m_clockIcon()
{
    // TODO title label, centered
}
