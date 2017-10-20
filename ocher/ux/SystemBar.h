/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_SYSTEMBAR_H
#define OCHER_UX_SYSTEMBAR_H

#include "ocher/ux/BatteryIcon.h"
#include "ocher/ux/ClockIcon.h"

#include <QWidget>

#include <string>

class SystemBar : public QWidget
{
    Q_OBJECT

public:
    SystemBar(Battery *battery);

    bool m_sep;

protected:
    BatteryIcon m_batteryIcon;
    ClockIcon m_clockIcon;
};

#endif
