/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/device/Device.h"
#include "ocher/settings/Settings.h"
#include "ocher/ux/PowerSaver.h"

#include <QtGlobal>


void PowerSaver::timeout()
{
    emit wantToSleep();
}

PowerSaver::PowerSaver() :
    m_seconds(15 * 60),
    // TODO settings
    m_device(0)
{
#if QT_VERSION >= 0x050000
    // Potentially more power efficient to not needlessly use fine-grained timer.
    m_timer.setTimerType(Qt::VeryCoarseTimer);
#endif
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void PowerSaver::inject(Device *device)
{
    m_device = device;
}

void PowerSaver::setTimeout(unsigned int seconds)
{
    m_seconds = seconds;
    resetTimeout();
}

void PowerSaver::resetTimeout()
{
    m_timer.start(m_seconds * 1000);
}

//void PowerSaver::dispatchEvent(const struct OcherEvent *evt)
//{
//    if (evt->type == OEVT_MOUSE) {
//        resetTimeout();
//    } else if (evt->type == OEVT_KEY) {
//        if (evt->key.key == OEVTK_POWER && evt->key.subtype == OEVT_KEY_UP)
//            timeout();
//        else
//            resetTimeout();
//    }
//}
//
//void PowerSaver::sleep()
//{
//    m_device->sleep();
//}
