/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_BATTERYICON_H
#define OCHER_UX_FB_BATTERYICON_H

class Battery;

class BatteryIcon
{
public:
    BatteryIcon(Battery *battery);

    Battery *m_battery;
    // TODO QPixmap ?
};

#endif
