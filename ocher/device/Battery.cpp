/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/device/Battery.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAT_PATH "/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/"


Battery::Battery() :
    m_percent(-1),
    m_status(Unknown)
{
    readAll();
}

int Battery::readAll()
{
    int r = 0;

    r |= readCapacity();
    r |= readStatus();
    return r;
}

int Battery::readCapacity()
{
    int r = -1;

#ifdef OCHER_TARGET_KOBO
    int fd = open(BAT_PATH "capacity", O_RDONLY);
    if (fd >= 0) {
        char buf[8];
        r = read(fd, buf, sizeof(buf) - 1);
        if (r > 0) {
            buf[r] = 0;
            m_percent = atoi(buf);
            r = 0;
        }
        close(fd);
    }
#endif
    if (r == -1)
        m_percent = -1;
    return r;
}

int Battery::readStatus()
{
#ifdef OCHER_TARGET_KOBO
    int r;
    char buf[5];
    int fd = open(BAT_PATH "status", O_RDONLY);
    if (fd < 0)
        goto unknown;
    r = read(fd, buf, 4);
    close(fd);
    if (r == 4) {
        buf[4] = 0;
        if (strcmp(buf, "Char") == 0) {
            m_status = Charging;
            return 0;
        } else if (strcmp(buf, "Disc") == 0) {
            m_status = Discharging;
            return 0;
        }
    }
unknown:
#endif
    m_status = Unknown;
    return -1;
}
